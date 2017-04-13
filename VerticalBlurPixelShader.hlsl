
#define MAX_BLUR_PIXELS 13

// Lighting information to sample from
Texture2D horizBlurTexture	: register(t0);
SamplerState blurSampler	: register(s0);

//extra data
cbuffer data : register(b0) {
	float blurDistance;	//how far from pixel to blur
	float texelSize; //dist to next pixel?
}

struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};

float4 main(TargetCoords input) : SV_TARGET	//replaces worldPos as they are not necessary anymore, and I do not think writing over what i am reading is good practice
{
	float blurD = clamp(blurDistance, 0, MAX_BLUR_PIXELS - 1);
	float weights[MAX_BLUR_PIXELS];
	float normalization = 0;

	for (int i = 0; i <= blurD; i++) {
		weights[i] = .045*pow(i*(4 / blurD), 3) - .269*pow(i*(4 / blurD), 2) + .129*(i*(4 / blurD)) + 1;
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
	float weight0, weight1, weight2, weight3, weight4;	//alphas of pixels by distance
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
	float weights[13];
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
		color += horizBlurTexture.Sample(blurSampler, input.uv + float2(0.0f, texelSize * i)) * weights[abs(i)];	//how to use diff weights -- need to calc weights depending on blurDistance
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

	return color;
}