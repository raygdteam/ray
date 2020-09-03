struct VertexInput
{
	float3 pos : POSITION;
	float2 texcoord0 : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 ViewProjMatrix;
}

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float2 texcoord0 : TEXCOORD;
};

VertexOutput main( VertexInput vertex )
{
	VertexOutput output;
	output.pos = mul(float4(vertex.pos, 1.f), ViewProjMatrix);
	output.texcoord0 = vertex.texcoord0;
	return output;
}