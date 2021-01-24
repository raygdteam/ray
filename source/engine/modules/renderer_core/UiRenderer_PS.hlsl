struct VertexOutput
{
	float4 pos : SV_POSITION;
	float2 texcoord0 : TEXCOORD;
	float4 color : COLOR;
	uint textureIndex : TEXINDEX;
};

#define GET_TEXTURE(X) case X: return Textures[X].Sample(DefaultSampler, input.texcoord0) * input.color;

Texture2D Textures[2] : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(VertexOutput input) : SV_TARGET
{
	switch (input.textureIndex)
	{
		GET_TEXTURE(0)
		GET_TEXTURE(1)
	}
	return 0.f;

}