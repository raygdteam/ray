struct VertexInput
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VertexOutput main( VertexInput vertex )
{
	VertexOutput output;
	output.pos = float4(vertex.pos, 1.f);
	output.color = vertex.color;
	return output;
}