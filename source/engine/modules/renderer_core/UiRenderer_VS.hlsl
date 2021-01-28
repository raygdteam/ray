struct VertexInput
{
	float2 pos : POSITION;
	float2 texcoord0 : TEXCOORD;
	float4 color : COLOR;
	uint textureIndex : TEXINDEX;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 ViewProjMatrix;
}

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float2 texcoord0 : TEXCOORD;
	float4 color : COLOR;
	uint textureIndex : TEXINDEX;
};

VertexOutput main(VertexInput vertex)
{
	VertexOutput output;
	output.pos = mul(float4(vertex.pos, 0.f, 1.f), ViewProjMatrix);
	output.color = vertex.color;
	output.texcoord0 = vertex.texcoord0;
	output.textureIndex = vertex.textureIndex;

	return output;
}