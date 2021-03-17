struct VertexInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
    uint instanceId : INSTANCE;
};

struct QuadInstance
{
    float4x4 Model;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 ViewProjMatrix;
}

StructuredBuffer<QuadInstance> Instances : register(t2);

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
};

float4x4 GetModelMatrix(uint instanceId)
{
    return Instances[instanceId].Model;
}

VertexOutput main( VertexInput vertex )
{
	VertexOutput output;
    float4x4 mvp = mul(GetModelMatrix(vertex.instanceId), ViewProjMatrix);
    output.pos = mul(vertex.pos, mvp);
	output.color = vertex.color;
	output.texcoord0 = vertex.texcoord0;
	output.textureIndex = vertex.textureIndex;

	return output;
}