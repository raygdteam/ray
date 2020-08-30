struct VertexInput
{
	float3 pos : POSITION;
	float2 texcoord0 : TEXCOORD0;
};

struct VertexOutput
{
	float4 pos : POSITION;
	float2 texcoord0 : TEXCOORD0;
};

VertexOutput main( VertexInput vertex )
{
	VertexOutput output;
	output.pos = float4(vertex.pos, 1.f);
	output.texcoord0 = vertex.texcoord0;
	return output;
}