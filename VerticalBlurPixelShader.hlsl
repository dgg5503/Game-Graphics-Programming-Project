
#define MAX_BLUR_PIXELS 26

// Lighting information to sample from
Texture2D horizBlurTexture	: register(t0);
SamplerState blurSampler	: register(s0);

//extra data
cbuffer data : register(b0) {
	float blurDistance;	//how far from pixel to blur
	float texelSize; //dist to next pixel?
}

static const float weights[MAX_BLUR_PIXELS] = { 0.040312,0.040111,0.039514,0.038539,0.037215,0.035579,0.033676,0.031559,0.02928,0.026896,0.024461,0.022024,0.019634,0.017328,0.015142,0.0131,0.01122,0.009515,0.007988,0.00664,0.005465,0.004453,0.003592,0.002869,0.002268,0.001776 };


struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};

struct BlurOutput
{
	float4 blurred	: SV_Target0;
};

float4 main(TargetCoords input) : SV_TARGET
{
	/*
	float blurD = clamp(blurDistance, 0, MAX_BLUR_PIXELS - 1);
	float weights[MAX_BLUR_PIXELS];
	float normalization = 0;

	for (int i = 0; i <= blurD; i++) {
		weights[i] = .045*pow(i*(4 / blurD), 3) - .269*pow(i*(4 / blurD), 2) + .129*(i*(4 / blurD)) + 1;
		weights[i] = clamp(weights[i], 0.0f, 1.0f);
		normalization += weights[i];
	}

	normalization -= weights[0];
	normalization *= 2.0f;
	normalization += weights[0];

	//normalize weights
	for (int j = 0; j <= blurD; j++) {
		weights[j] = weights[j] / normalization;
	}
	*/
	//add the weighted colors together
	float4 color = float4(0, 0, 0, 0);
	for (int k = -blurDistance; k <= blurDistance; k++) {
		color += horizBlurTexture.Sample(blurSampler, input.uv + float2(0.0f, texelSize * k)) * weights[abs(clamp(k, 0, MAX_BLUR_PIXELS - 1))];	//how to use diff weights -- need to calc weights depending on blurDistance
	}


	//set alpha to 1
	color.a = 1.0f;

	return color;
}