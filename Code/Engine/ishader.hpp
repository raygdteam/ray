#pragma once
class IShader
{
public:
	IShader();
	virtual ~IShader() = 0;

	virtual void Compile() = 0;
	virtual void SetSources(std::string&, std::string&) = 0;
	virtual void SetSources(pcstr, pcstr) = 0;
	virtual void UseShader() = 0;
	virtual void Destroy() = 0;
};

