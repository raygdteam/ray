#pragma once
class IShader
{
public:
	IShader();
	virtual ~IShader() = 0;

	virtual void Compile() = 0;
	virtual void SetVertexShader(ray_string&) = 0;
	virtual void SetPixelShader(ray_string&) = 0;
	virtual u32 GetProgram() = 0;
};

