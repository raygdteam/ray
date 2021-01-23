struct VertexOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord0 : TEXCOORD;
	uint textureIndex : TEXINDEX;
};

#define GET_TEXTURE(X) case X: return Textures[X].Sample(DefaultSampler, input.texcoord0) * input.color;

Texture2D Textures[64] : register(t0);
SamplerState DefaultSampler : register(s0);

float4 main(VertexOutput input) : SV_TARGET
{
	switch (input.textureIndex)
	{
	case 0: return Textures[0].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 1: return Textures[1].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 2: return Textures[2].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 3: return Textures[3].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 4: return Textures[4].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 5: return Textures[5].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 6: return Textures[6].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 7: return Textures[7].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 8: return Textures[8].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 9: return Textures[9].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 10: return Textures[10].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 11: return Textures[11].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 12: return Textures[12].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 13: return Textures[13].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 14: return Textures[14].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 15: return Textures[15].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 16: return Textures[16].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 17: return Textures[17].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 18: return Textures[18].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 19: return Textures[19].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 20: return Textures[20].Sample(DefaultSampler, input.texcoord0) * input.color; 
	case 21: return Textures[21].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 22: return Textures[22].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 23: return Textures[23].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 24: return Textures[24].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 25: return Textures[25].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 26: return Textures[26].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 27: return Textures[27].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 28: return Textures[28].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 29: return Textures[29].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 30: return Textures[30].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 31: return Textures[31].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 32: return Textures[32].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 33: return Textures[33].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 34: return Textures[34].Sample(DefaultSampler, input.texcoord0) * input.color; 
	case 35: return Textures[35].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 36: return Textures[36].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 37: return Textures[37].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 38: return Textures[38].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 39: return Textures[39].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 40: return Textures[40].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 41: return Textures[41].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 42: return Textures[42].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 43: return Textures[43].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 44: return Textures[44].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 45: return Textures[45].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 46: return Textures[46].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 47: return Textures[47].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 48: return Textures[48].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 49: return Textures[49].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 50: return Textures[50].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 51: return Textures[51].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 52: return Textures[52].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 53: return Textures[53].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 54: return Textures[54].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 55: return Textures[55].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 56: return Textures[56].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 57: return Textures[57].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 58: return Textures[58].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 59: return Textures[59].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 60: return Textures[60].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 61: return Textures[61].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 62: return Textures[62].Sample(DefaultSampler, input.texcoord0) * input.color;
	case 63: return Textures[63].Sample(DefaultSampler, input.texcoord0) * input.color;
		 
	}
	return 0.f;

}