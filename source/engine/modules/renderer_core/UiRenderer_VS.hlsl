struct VertexInput
{
	float2 pos : POSITION;
	float2 texcoord0 : TEXCOORD;
	float4 color : COLOR;
	uint textureIndex : TEXINDEX;
};

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
	output.pos = float4(vertex.pos, 1.f, 1.f);
	output.color = vertex.color;
	output.texcoord0 = vertex.texcoord0;
	output.textureIndex = vertex.textureIndex;

	return output;
}