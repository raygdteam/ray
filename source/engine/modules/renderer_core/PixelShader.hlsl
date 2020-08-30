struct VertexOutput
{
	float4 pos : SV_POSITION;
	float2 texcoord0 : TEXCOORD0;
};

Texture2D TextureAtlas : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(VertexOutput input) : SV_TARGET
{
	return TextureAtlas.Sample(DefaultSampler, input.texcoord0);
}