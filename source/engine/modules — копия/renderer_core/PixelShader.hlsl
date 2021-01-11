struct VertexOutput
{
	float4 pos : SV_POSITION;
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
};

#define GET_TEXTURE(X) case X: returnTextureAtlas[X].Sample(DefaultSampler, input.texcoord0);

Texture2D TextureAtlas[64] : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(VertexOutput input) : SV_TARGET
{
	switch (input.textureIndex)
	{
	case 0: return TextureAtlas[0].Sample(DefaultSampler, input.texcoord0);
	case 1: return TextureAtlas[1].Sample(DefaultSampler, input.texcoord0);
	case 2: return TextureAtlas[2].Sample(DefaultSampler, input.texcoord0);
	case 3: return TextureAtlas[3].Sample(DefaultSampler, input.texcoord0);
	case 4: return TextureAtlas[4].Sample(DefaultSampler, input.texcoord0);
	case 5: return TextureAtlas[5].Sample(DefaultSampler, input.texcoord0);
	case 6: return TextureAtlas[6].Sample(DefaultSampler, input.texcoord0);
	case 7: return TextureAtlas[7].Sample(DefaultSampler, input.texcoord0);
	case 8: return TextureAtlas[8].Sample(DefaultSampler, input.texcoord0);
	case 9: return TextureAtlas[9].Sample(DefaultSampler, input.texcoord0);
	case 10: return TextureAtlas[10].Sample(DefaultSampler, input.texcoord0);
	case 11: return TextureAtlas[11].Sample(DefaultSampler, input.texcoord0);
	case 12: return TextureAtlas[12].Sample(DefaultSampler, input.texcoord0);
	case 13: return TextureAtlas[13].Sample(DefaultSampler, input.texcoord0);
	case 14: return TextureAtlas[14].Sample(DefaultSampler, input.texcoord0);
	case 15: return TextureAtlas[15].Sample(DefaultSampler, input.texcoord0);
	case 16: return TextureAtlas[16].Sample(DefaultSampler, input.texcoord0);
	case 17: return TextureAtlas[17].Sample(DefaultSampler, input.texcoord0);
	case 18: return TextureAtlas[18].Sample(DefaultSampler, input.texcoord0);
	case 19: return TextureAtlas[19].Sample(DefaultSampler, input.texcoord0);
	case 20: return TextureAtlas[20].Sample(DefaultSampler, input.texcoord0); 
	case 21: return TextureAtlas[21].Sample(DefaultSampler, input.texcoord0);
	case 22: return TextureAtlas[22].Sample(DefaultSampler, input.texcoord0);
	case 23: return TextureAtlas[23].Sample(DefaultSampler, input.texcoord0);
	case 24: return TextureAtlas[24].Sample(DefaultSampler, input.texcoord0);
	case 25: return TextureAtlas[25].Sample(DefaultSampler, input.texcoord0);
	case 26: return TextureAtlas[26].Sample(DefaultSampler, input.texcoord0);
	case 27: return TextureAtlas[27].Sample(DefaultSampler, input.texcoord0);
	case 28: return TextureAtlas[28].Sample(DefaultSampler, input.texcoord0);
	case 29: return TextureAtlas[29].Sample(DefaultSampler, input.texcoord0);
	case 30: return TextureAtlas[30].Sample(DefaultSampler, input.texcoord0);
	case 31: return TextureAtlas[31].Sample(DefaultSampler, input.texcoord0);
	case 32: return TextureAtlas[32].Sample(DefaultSampler, input.texcoord0);
	case 33: return TextureAtlas[33].Sample(DefaultSampler, input.texcoord0);
	case 34: return TextureAtlas[34].Sample(DefaultSampler, input.texcoord0); 
	case 35: return TextureAtlas[35].Sample(DefaultSampler, input.texcoord0);
	case 36: return TextureAtlas[36].Sample(DefaultSampler, input.texcoord0);
	case 37: return TextureAtlas[37].Sample(DefaultSampler, input.texcoord0);
	case 38: return TextureAtlas[38].Sample(DefaultSampler, input.texcoord0);
	case 39: return TextureAtlas[39].Sample(DefaultSampler, input.texcoord0);
	case 40: return TextureAtlas[40].Sample(DefaultSampler, input.texcoord0);
	case 41: return TextureAtlas[41].Sample(DefaultSampler, input.texcoord0);
	case 42: return TextureAtlas[42].Sample(DefaultSampler, input.texcoord0);
	case 43: return TextureAtlas[43].Sample(DefaultSampler, input.texcoord0);
	case 44: return TextureAtlas[44].Sample(DefaultSampler, input.texcoord0);
	case 45: return TextureAtlas[45].Sample(DefaultSampler, input.texcoord0);
	case 46: return TextureAtlas[46].Sample(DefaultSampler, input.texcoord0);
	case 47: return TextureAtlas[47].Sample(DefaultSampler, input.texcoord0);
	case 48: return TextureAtlas[48].Sample(DefaultSampler, input.texcoord0);
	case 49: return TextureAtlas[49].Sample(DefaultSampler, input.texcoord0);
	case 50: return TextureAtlas[50].Sample(DefaultSampler, input.texcoord0);
	case 51: return TextureAtlas[51].Sample(DefaultSampler, input.texcoord0);
	case 52: return TextureAtlas[52].Sample(DefaultSampler, input.texcoord0);
	case 53: return TextureAtlas[53].Sample(DefaultSampler, input.texcoord0);
	case 54: return TextureAtlas[54].Sample(DefaultSampler, input.texcoord0);
	case 55: return TextureAtlas[55].Sample(DefaultSampler, input.texcoord0);
	case 56: return TextureAtlas[56].Sample(DefaultSampler, input.texcoord0);
	case 57: return TextureAtlas[57].Sample(DefaultSampler, input.texcoord0);
	case 58: return TextureAtlas[58].Sample(DefaultSampler, input.texcoord0);
	case 59: return TextureAtlas[59].Sample(DefaultSampler, input.texcoord0);
	case 60: return TextureAtlas[60].Sample(DefaultSampler, input.texcoord0);
	case 61: return TextureAtlas[61].Sample(DefaultSampler, input.texcoord0);
	case 62: return TextureAtlas[62].Sample(DefaultSampler, input.texcoord0);
	case 63: return TextureAtlas[63].Sample(DefaultSampler, input.texcoord0);
		 
	}
	return 0.f;

}