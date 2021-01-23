struct VertexInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 ViewProjMatrix;
}

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
};

VertexOutput main( VertexInput vertex )
{
	VertexOutput output;
	output.pos = mul(vertex.pos, ViewProjMatrix);
	output.color = vertex.color;
	output.texcoord0 = vertex.texcoord0;
	output.textureIndex = vertex.textureIndex;

	return output;
}