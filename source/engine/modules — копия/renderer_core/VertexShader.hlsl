struct VertexInput
{
	float3 pos : POSITION;
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
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
};

VertexOutput main( VertexInput vertex )
{
	VertexOutput output;
	output.pos = mul(float4(vertex.pos, 1.f), ViewProjMatrix);
	//output.pos = float4(vertex.pos, 1.f);
	output.texcoord0 = vertex.texcoord0;
	output.textureIndex = vertex.textureIndex;

	return output;
}