
#define MAX_BLUR_PIXELS 13

// Adapted from http://www.rastertek.com/dx11tut36.html
// Lighting information to sample from
Texture2D blurTexture		: register(t0);
SamplerState blurSampler	: register(s0);

//extra data
cbuffer data : register(b0) {
	float blurDistance;	//how far from pixel to blur
	float texelSize; //dist to next pixel -->calculated in code b/c only need to calculate once or when resized, not every pixel/draw/etc.
}

// Input info from vertex shader
struct BlurPixelInput
{
	float4 position		: SV_POSITION;
	float2 tex			: TEXCOORD0;	//origin coord
	float2 texCoord1	: TEXCOORD1;	//coordinates 4 to the left and 4 to the right to be written to for blur
	float2 texCoord2	: TEXCOORD2;	//technically only need the original tex position
	float2 texCoord3	: TEXCOORD3;
	float2 texCoord4	: TEXCOORD4;
	float2 texCoord5	: TEXCOORD5;
	float2 texCoord6	: TEXCOORD6;
	float2 texCoord7	: TEXCOORD7;
	float2 texCoord8	: TEXCOORD8;
	float2 texCoord9	: TEXCOORD9;
};

struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};


float4 main(TargetCoords input) : SV_TARGET	
{
	float blurD = clamp(blurDistance, 0, MAX_BLUR_PIXELS - 1);
	float weights[MAX_BLUR_PIXELS];
	/*
	weights[0] = weight0;
	weights[1] = weight1;
	weights[2] = weight2;
	weights[3] = weight3;
	weights[4] = weight4;
	weights[5] = weight5;
	weights[6] = weight6;
	weights[7] = weight7;
	weights[8] = weight8;
	weights[9] = weight9;
	weights[10] = weight10;
	weights[11] = weight11;
	weights[12] = weight12;
	*/
	float normalization = 0;
	//float weight0, weight1, weight2, weight3, weight4;

	//all of these are only calculated once or when blurDistance changes...better in here or in the cbuffer?
	//weight function .045x^3 - .269x^2 + .129x + 1
	for (int i = 0; i <= blurD; i++) {
		weights[i] = .045*pow(i*(4/ blurD), 3) - .269*pow(i*(4 / blurD), 2) + .129*(i*(4 / blurD)) + 1;
		weights[i] = clamp(weights[i], 0.1f, 1.0f);
		normalization += weights[i];
	}

	normalization -= weights[0];
	normalization *= 2.0f;
	normalization += weights[0];

	//normalize weights
	for (int i = 0; i <= blurD; i++) {
		weights[i] = weights[i] / normalization;
	}


	/*
	weight0 = 1.0f;	//always this
	weight1 = 0.9f;	//changes
	weight2 = 0.55f;	//at half
	weight3 = 0.18f;	//changes
	weight4 = 0.1f;	//at furthest

	float normalization;
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));	//average the weights a bit
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	//calc once in renderer and pass in?
	float weights[blurDistance];
	weights[0] = weight0;
	weights[1] = weight1;
	weights[2] = weight2;
	weights[3] = weight3;
	weights[4] = weight4;
	for (int j = 5; j < blurDistance; j++) {
		weights[j] = weight4;	//set as original weight?
	}
	*/
	//add the weighted colors together
	float4 color = float4(0, 0, 0, 0);
	for (int i = -blurD; i <= blurD; i++) {
		color += blurTexture.Sample(blurSampler, input.uv + float2(texelSize * i, 0.0f)) * weights[abs(i)];	//how to use diff weights -- need to calc weights depending on blurDistance
	}
	/*
	color += blurTexture.Sample(blurSampler, input.texCoord1) * weight4;
	color += blurTexture.Sample(blurSampler, input.texCoord2) * weight3;
	color += blurTexture.Sample(blurSampler, input.texCoord3) * weight2;
	color += blurTexture.Sample(blurSampler, input.texCoord4) * weight1;
	color += blurTexture.Sample(blurSampler, input.texCoord5) * weight0;
	color += blurTexture.Sample(blurSampler, input.texCoord6) * weight1;
	color += blurTexture.Sample(blurSampler, input.texCoord7) * weight2;
	color += blurTexture.Sample(blurSampler, input.texCoord8) * weight3;
	color += blurTexture.Sample(blurSampler, input.texCoord9) * weight4;
	*/

	//set alpha to 1
	color.a = 1.0f;
	//second shader should blur vertically
	return color;
}