struct VertexInput
{
	float3 pos : POSITION;
	float2 texcoord0 : TEXCOORD;
};

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float2 texcoord0 : TEXCOORD;
};

VertexOutput main( VertexInput vertex )
{
	VertexOutput output;
	output.pos = float4(vertex.pos, 1.f);
	output.texcoord0 = vertex.texcoord0;
	return output;
}