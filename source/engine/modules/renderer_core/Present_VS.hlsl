struct VertexInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VertexOutput main( VertexInput vertex )
{
    VertexOutput output;
    output.pos = float4(vertex.pos, 1.f);
    output.texCoord = vertex.texCoord;
    
	return output;
}