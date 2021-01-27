struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

Texture2D FinalFrame : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(VertexOutput vertex) : SV_TARGET
{
    return FinalFrame.Sample(DefaultSampler, vertex.texCoord);
}