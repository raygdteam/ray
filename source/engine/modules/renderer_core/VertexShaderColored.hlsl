struct VertexInput
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 ViewProjMatrix;
}

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VertexOutput main(VertexInput vertex)
{
	VertexOutput output;
	output.pos = mul(float4(vertex.pos, 1.f), ViewProjMatrix);
	//output.pos = float4(vertex.pos, 1.f);
	output.color = vertex.color;

	return output;
}