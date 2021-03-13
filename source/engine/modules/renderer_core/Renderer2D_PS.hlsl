struct VertexOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
};

#define GET_TEXTURE(X) case X: return Textures[X].Sample(DefaultSampler, input.texcoord0) * input.color;

Texture2D Textures[16] : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(VertexOutput input) : SV_TARGET
{
	switch (input.textureIndex)
	{
		GET_TEXTURE(0)
		GET_TEXTURE(1)
		GET_TEXTURE(2)
		GET_TEXTURE(3)
		GET_TEXTURE(4)
		GET_TEXTURE(5)
		GET_TEXTURE(6)
		GET_TEXTURE(7)
		GET_TEXTURE(8)
		GET_TEXTURE(9)
		GET_TEXTURE(10)
		GET_TEXTURE(11)
		GET_TEXTURE(12)
		GET_TEXTURE(13)
		GET_TEXTURE(14)
		GET_TEXTURE(15)
	}
	return 0.f;

}