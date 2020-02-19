#include "pch.hpp"
#include "file_system.hpp"
#include "platform/platform_fs.hpp"

namespace ray
{

class FileMappedArchive : public serialization::IArchive
{
	core::platform::PlatformFileHandle _handle;
	size_t _currentPos;

	void Write(void* data, size_t size)
	{
		// я дебил
	}

	template<typename T>
	T Read()
	{
		static_assert(sizeof T > 0);

		auto size = sizeof T;
		auto pos = _currentPos;
		
		_currentPos += size;

		return *(T*)_handle.Buffer + pos;
	}

public:
	FileMappedArchive(core::platform::PlatformFileHandle handle) : _handle(handle), _currentPos(0)
	{}

	template<typename T>
	IArchive operator<<(T t)
	{
		t = Read<T>();
		return *this;
	}

	template<typename T>
	IArchive operator>>(T t)
	{
		return *this;
	}
};

void FileSystem::initialize()
{
	core::platform::PlatformFS::Initialize();
}

serialization::IArchive FileSystem::ReadFile(pcstr path)
{
	return *(serialization::IArchive*)nullptr;
}


FileSystem FileSystem::Get()
{
	static FileSystem fileSystem;
	return fileSystem;
}

}
