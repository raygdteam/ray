#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../../Engine/ishader.hpp"

class ShaderDX :
	public IShader
{
public:
	ShaderDX();
	~ShaderDX() override;

	void Compile() override;
	void SetVertexShader(ray_string&) override;
	void SetPixelShader(ray_string&) override;
	u32 GetProgram() override { return (u32)0; }

	void SetVertexBlob(ID3DBlob** Blob) { m_VertexBlob = Blob; }
	void SetPixelBlob(ID3DBlob** Blob) { m_PixelBlob = Blob; }
	static void SetDevice(ID3D11Device* Device) { m_sDevice = Device; }

	ID3D11VertexShader* GetVertexShader() const { return m_VertexShader; }
	ID3D11PixelShader* GetPixelShader() const { return m_PixelShader; }

private:
	HRESULT _Compile(const WCHAR*, LPCSTR, LPCSTR, ID3DBlob**);

private:
	ray_string m_VertexSource;
	ray_string m_PixelSource;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3DBlob** m_VertexBlob;
	ID3DBlob** m_PixelBlob;

	static ID3D11Device* m_sDevice;

};

