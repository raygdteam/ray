#pragma once

namespace ray::serialization
{

class IArchive
{
protected:
	void Write(void* data, size_t size);

	template<typename T>
	T Read();
public:
	virtual ~IArchive() = default;

	template<typename T>
	IArchive operator<<(T t)
	{
		return *this;
	}

	template<typename T>
	IArchive operator>>(T t)
	{
		return *this;
	}
};

}