#pragma once
#include "core/pch.hpp"
#include "core/core.hpp"
#include "core/serialization/archive.hpp"
#include "serialization/stream.hpp"

namespace ray 
{

    /**
     * Defines the device file system api.
     */
    class RAY_CORE_API FileSystem
    {
    public:

        enum class AccessFlags : uint32_t
        {
            eRead = 1,
            eWrite = 2
        };

        /**
         * Destructor.
         */
        ~FileSystem();

        /**
         * Sets the home path for the game.
         *
         * Once set, all file loading will load from the given home path.
         *
         * @param path The path to the game home folder.
         */
        static void SetHomePath(const string& path);

        /**
         * Gets the currently set asset path.
         *
         * @return The currently set asset path.
         */
        static string GetHomePath();

        /**
         * Gets the absolute directory for a relative path or "" if not resolved.
         *
         * @param relativePath The relative path to be resolved.
         * @return The absolute directory for a relative path or "" if not resolved.
         */
        static string GetAbsolutePath(const string& relativePath);

        /**
         * Resolves a filesystem path.
         *
         * If the specified path is a file system alias, the alias will be
         * resolved and the physical file will be returned.
         *
         * @param path The path to resolve.
         * @return The resolved file path.
         */
        static string ResolvePath(const string& path);

        /**
         * Lists the files in the specified directory and adds the files to the vector. Excludes directories.
         *
         * @param dirPath Directory path relative to the path set in <code>setHomePath(const char*)</code>.
         * @param files The vector to append the files to.
         * @return True if successful, false if error.
         */
        static bool ListFiles(const string& dirPath, std::vector<string>& files);

        /**
         * Checks if the file at the given path exists.
         *
         * @param dropFile The path to the file.
         * @return <code>true</code> if the file exists; <code>false</code> otherwise.
         */
        static bool FileExists(const string& dropFile);

        /**
         * Opens a byte stream for the given asset path.
         * @param path Relative path to be opened.
         * @param accessFlags The access flags used to open the file.
         * @return A stream that can be used to read or write to the file depending on the mode,
         *         nullptr if there was an error. (Request mode not supported).
         */
        static quantum::Stream* Open(const string& path, AccessFlags accessFlags = AccessFlags::eRead);

        /**
         * Reads the entire contents of the specified file and returns its contents.
         *
         * The returned character array is allocated with new[] and must therefore
         * deleted by the caller using delete[].
         *
         * @param dropFile The path to the file to be read.
         * @return A string containing the contents of the file or empty string if
         */
        static string ReadAll(const string& dropFile);

        /**
         * Determines if the file path is an absolute path for the current platform.
         *
         * @param dropFile The file path to test.
         * @return true if the path is an absolute path or false otherwise.
         */
        static bool IsAbsolutePath(const string& dropFile);

        /**
         * Gets the directory name up to and including the trailing '/'.
         *
         * This is a lexical method so it does not verify that the directory exists.
         * Back slashes will be converted to forward slashes.
         *
         * - "res/image.png" will return "res/"
         * - "image.png" will return ""
         * - "c:/foo/bar/image.png" will output "c:/foo/bar/"
         *
         * @param path The file path. May be relative or absolute, forward or back slashes. May be nullptr.
         * @return The directory name with the trailing '/' or emptry string if invalid string or directory.
         */
        static string GetDirectoryPath(const string& path);

        /**
         * Gets the extension of the given file path.
         *
         * The extension returned includes all character after and including the last '.'
         * in the file path. The extension is returned as all lowercase.
         *
         * If the path does not contain an extension, an empty string is returned.
         *
         * @param path File path.
         * @return The file extension, all uppercase, including the '.'.
         */
        static string GetExtension(const string& path);

    private:

        FileSystem();
    };

}

RAY_ENABLE_BITWISE_OPERATORS(ray::FileSystem::AccessFlags)
