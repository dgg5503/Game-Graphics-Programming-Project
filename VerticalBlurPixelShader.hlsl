
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

struct BlurOutput
{
	float4 blurred	: SV_Target0;
};

float4 main(TargetCoords input) : SV_TARGET
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
	for (int j = 0; j <= blurD; j++) {
		weights[j] = weights[j] / normalization;
	}

	//add the weighted colors together
	float4 color = float4(0, 0, 0, 0);
	for (int k = -blurD; k <= blurD; k++) {
		color += horizBlurTexture.Sample(blurSampler, input.uv + float2(0.0f, texelSize * k)) * weights[abs(k)];	//how to use diff weights -- need to calc weights depending on blurDistance
	}


	//set alpha to 1
	color.a = 1.0f;

	return color;
}