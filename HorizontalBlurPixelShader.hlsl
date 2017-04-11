

// Adapted from http://www.rastertek.com/dx11tut36.html
// Lighting information to sample from
Texture2D emissionTexture	: register(t3);
SamplerState blurSampler	: register(s0);

// Input info from vertex shader
struct BlurPixelInput
{
	float4 position		: SV_POSITION;
	float2 tex			: TEXCOORD0;	//origin coord
	float2 texCoord1	: TEXCOORD1;	//coordinates 4 to the left and 4 to the right to be written to for blur
	float2 texCoord2	: TEXCOORD2;
	float2 texCoord3	: TEXCOORD3;
	float2 texCoord4	: TEXCOORD4;
	float2 texCoord5	: TEXCOORD5;
	float2 texCoord6	: TEXCOORD6;
	float2 texCoord7	: TEXCOORD7;
	float2 texCoord8	: TEXCOORD8;
	float2 texCoord9	: TEXCOORD9;
};

//If the color is black, pretend it is fully transparent black -- ick to the if statement again
float4 GetEmissionPixel(float2 uv) {
	float4 color = emissionTexture.Sample(blurSampler, uv);
	if (length(color.xyz) == 0) {
		return float4(0, 0, 0, 0);
	}
	return color;
};

float4 main(BlurPixelInput input) : SV_TARGET	//replaces normals as they are not necessary anymore --> final shader should alpha blend the vert blur(which uses this texture we made) and the colors with lighting tex, alphablend necessary or done in shader? (shader may be preferred)
{
	//output should be the horizontal blurred texture of the emission
	float weight0, weight1, weight2, weight3, weight4;	//alphas of pixels by distance
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	float normalization;
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));	//average the weights a bit
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	//add the weighted colors together
	float4 color = float4(0, 0, 0, 0);
	color += GetEmissionPixel(input.texCoord1) * weight4;
	color += GetEmissionPixel(input.texCoord2) * weight3;
	color += GetEmissionPixel(input.texCoord3) * weight2;
	color += GetEmissionPixel(input.texCoord4) * weight1;
	color += GetEmissionPixel(input.texCoord5) * weight0;
	color += GetEmissionPixel(input.texCoord6) * weight1;
	color += GetEmissionPixel(input.texCoord7) * weight2;
	color += GetEmissionPixel(input.texCoord8) * weight3;
	color += GetEmissionPixel(input.texCoord9) * weight4;

	//I think this should have some alpha...maybe
	//second shader should blur vertically
	return color;
}