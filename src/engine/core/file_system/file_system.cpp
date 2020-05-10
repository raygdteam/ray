#include "pch.hpp"
#include "core.hpp"
#include "file_system.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include <map>
#include <cctype>
#ifdef RAY_PLATFORM_WIN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <direct.h>
#define _stat _stat
#define _stat_struct struct stat
#else
#define __EXT_POSIX2
#include <libgen.h>
#include <dirent.h>
#define _stat stat
#define _stat_struct struct stat
#endif

namespace ray
{
    static string __homePath(".");
    static std::map<string, string> __aliases;

    static void getFullPath(const string& path, string& fullPath)
    {
        if (ray::FileSystem::IsAbsolutePath(path))
        {
            fullPath.assign(path);
        }
        else
        {
            fullPath.assign(__homePath);
            fullPath += FileSystem::ResolvePath(path);
        }
    }

    /**
     *  File stream.
     */
    class FileStream : public quantum::Stream
    {
    public:
        friend class FileSystem;

        ~FileStream();
        bool CanRead() override;
        bool CanWrite() override;
        bool CanSeek() override;
        void Close() override;
        size_t Read(void* ptr, size_t size, size_t count) override;
        char* ReadLine(char* str, int num) override;
        size_t Write(const void* ptr, size_t size, size_t count) override;
        bool Eof() override;
        size_t Length() override;
        long int Position() override;
        bool Seek(long int offset, int origin) override;
        bool Rewind() override;

        static FileStream* create(const char* filePath, const char* mode);

    private:
        FileStream(FILE* file);

    private:
        FILE* _file;
        bool _canRead;
        bool _canWrite;
    };


    FileSystem::FileSystem()
    {
    }

    FileSystem::~FileSystem()
    {
    }

    void FileSystem::SetHomePath(const string& path)
    {
        if (path.length() > 0)
            __homePath = path;
    }

    string FileSystem::GetHomePath()
    {
        return __homePath;
    }

    string FileSystem::GetAbsolutePath(const string& path)
    {
        char fullPath[_MAX_PATH];
        if (_fullpath(fullPath, path.c_str(), _MAX_PATH) != nullptr)
        {
            return string(fullPath);
        }

        return "";
    }

    string FileSystem::ResolvePath(const string& path)
    {
        if (path.length() > 1 && path.at(0) == '@')
        {
            string alias = path.substr(1);
            std::map<string, string>::const_iterator itr = __aliases.find(alias);
            if (itr == __aliases.end())
                return path;
            return itr->second.c_str();
        }
        return __homePath + path;
    }

    bool FileSystem::ListFiles(const string& dirPath, std::vector<string>& files)
    {
#ifdef RAY_PLATFORM_WIN
        string path(FileSystem::GetHomePath());
        if (dirPath.length() > 0)
        {
            path.append(dirPath);
        }
        path.append("/*");
        // Convert char to wchar
        std::basic_string<TCHAR> wPath;
        wPath.assign(path.begin(), path.end());

        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = FindFirstFile(wPath.c_str(), &FindFileData);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        do
        {
            // Add to the list if this is not a directory
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                // Convert wchar to char
                std::basic_string<TCHAR> wfilename(FindFileData.cFileName);
                string filename;
                filename.assign(wfilename.begin(), wfilename.end());
                files.push_back(filename);
            }
        } while (FindNextFile(hFind, &FindFileData) != 0);

        FindClose(hFind);
        return true;
#else
        string path(FileSystem::getHomePath());
        if (dirPath.length() > 0)
        {
            path.append(dirPath);
        }
        path.append("/.");
        bool result = false;

        struct dirent* dp;
        DIR* dir = opendir(path.c_str());
        if (dir != nullptr)
        {
            while ((dp = readdir(dir)) != nullptr)
            {
                string filepath(path);
                filepath.append("/");
                filepath.append(dp->d_name);

                struct stat buf;
                if (!stat(filepath.c_str(), &buf))
                {
                    // Add to the list if this is not a directory
                    if (!S_ISDIR(buf.st_mode))
                    {
                        files.push_back(dp->d_name);
                    }
                }
            }
            closedir(dir);
            result = true;
        }

        return result;
#endif
    }

    bool FileSystem::FileExists(const string& filePath)
    {
        string fullPath;
        getFullPath(filePath, fullPath);
        _stat_struct s;
        return stat(fullPath.c_str(), &s) == 0;
    }

	quantum::Stream* FileSystem::Open(const string& path, AccessFlags accessFlags)
    {
        char modeStr[] = "rb";
        if (bool(static_cast<u32>(accessFlags) & static_cast<u32>(AccessFlags::eWrite)))
        {
            modeStr[0] = 'w';
        }
        string fullPath;
        getFullPath(path, fullPath);
        FileStream* stream = FileStream::create(fullPath.c_str(), modeStr);
        return stream;
    }

    string FileSystem::ReadAll(const string& filePath)
    {
        // Open file for reading.
        std::unique_ptr<quantum::Stream> stream(Open(filePath));
        if (stream.get() == nullptr)
        {
            // ERROR("Failed to load file: %s", filePath.c_str());
        }
        size_t size = stream->Length();
        string result;
        result.resize(size);
        size_t read = stream->Read(&result[0], 1, size);
        if (read != size)
        {
            // QUANTUM_ERROR("Failed to read complete contents of file '%s' (amount read vs. file size: %u < %u).", filePath.c_str(), read, size);
        }
        return result;
    }

    bool FileSystem::IsAbsolutePath(const string& filePath)
    {
        char first = static_cast<char>(::toupper(static_cast<s32>(filePath.at(0))));
        char second = filePath.at(1);
        return (second == ':' && (first >= 'A' && first <= 'Z'));
    }

    string FileSystem::GetDirectoryPath(const string& path)
    {
        if (path.length() == 0)
            return "";
        char drive[_MAX_DRIVE];
        char dir[_MAX_DIR];
        _splitpath(path.c_str(), drive, dir, nullptr, nullptr);
        string dirname;
        size_t driveLength = strlen(drive);
        if (driveLength > 0)
        {
            dirname.reserve(driveLength + strlen(dir));
            dirname.append(drive);
            dirname.append(dir);
        }
        else
        {
            dirname.assign(dir);
        }
        std::replace(dirname.begin(), dirname.end(), '\\', '/');
        return dirname;

    }

    string FileSystem::GetExtension(const string& path)
    {
        string str = path.substr(path.find_last_of('.') + 1);
        if (str.length() == 0)
            return "";

        string ext;
        for (size_t i = 0; i < str.length(); ++i)
            ext += std::tolower(str.at(i));

        return ext;
    }


    FileStream::FileStream(FILE* file)
        : _file(file), _canRead(false), _canWrite(false)
    {
    }

    FileStream::~FileStream()
    {
        if (_file)
            this->Close();
    }

    FileStream* FileStream::create(const char* filePath, const char* mode)
    {
#ifdef RAY_PLATFORM_WIN
        /* replace all "/"'s in the path with "\\" for Windows platform. */
        string fixedPath(filePath);
        fixedPath = std::regex_replace(fixedPath, std::regex("/"), "\\");
#endif

        FILE* file;
        fopen_s(&file, fixedPath.c_str(), mode);
        if (file)
        {
            FileStream* stream = new FileStream(file);
            const char* s = mode;
            while (s != nullptr && *s != '\0')
            {
                if (*s == 'r')
                    stream->_canRead = true;
                else if (*s == 'w')
                    stream->_canWrite = true;
                ++s;
            }

            return stream;
        }
        return nullptr;
    }

    bool FileStream::CanRead()
    {
        return _file && _canRead;
    }

    bool FileStream::CanWrite()
    {
        return _file && _canWrite;
    }

    bool FileStream::CanSeek()
    {
        return _file != nullptr;
    }

    void FileStream::Close()
    {
        if (_file)
            fclose(_file);
        _file = nullptr;
    }

    size_t FileStream::Read(void* ptr, size_t size, size_t count)
    {
        if (!_file)
            return 0;
        return fread(ptr, size, count, _file);
    }

    char* FileStream::ReadLine(char* str, int num)
    {
        if (!_file)
            return 0;
        return fgets(str, num, _file);
    }

    size_t FileStream::Write(const void* ptr, size_t size, size_t count)
    {
        if (!_file)
            return 0;
        return fwrite(ptr, size, count, _file);
    }

    bool FileStream::Eof()
    {
        if (!_file || feof(_file))
            return true;
        return ((size_t)Position()) >= Length();
    }

    size_t FileStream::Length()
    {
        size_t len = 0;
        if (CanSeek())
        {
            long int pos = Position();
            if (Seek(0, SEEK_END))
            {
                len = Position();
            }
            Seek(pos, SEEK_SET);
        }
        return len;
    }

    long int FileStream::Position()
    {
        if (!_file)
            return -1;
        return ftell(_file);
    }

    bool FileStream::Seek(long int offset, int origin)
    {
        if (!_file)
            return false;
        return fseek(_file, offset, origin) == 0;
    }

    bool FileStream::Rewind()
    {
        if (CanSeek())
        {
            ::rewind(_file);
            return true;
        }
        return false;
    }

}
