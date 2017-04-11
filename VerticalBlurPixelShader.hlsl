


// Lighting information to sample from
Texture2D horizBlurTexture	: register(t1);
Texture2D emissionTexture	: register(t3);
SamplerState blurSampler	: register(s0);

// Input info from vertex shader
struct BlurPixelInput
{
	float4 position		: SV_POSITION;
	float2 tex			: TEXCOORD0;	//origin coord
	float2 texCoord1	: TEXCOORD1;	//coordinates 4 up and 4 down to be written to for blur
	float2 texCoord2	: TEXCOORD2;
	float2 texCoord3	: TEXCOORD3;
	float2 texCoord4	: TEXCOORD4;
	float2 texCoord5	: TEXCOORD5;
	float2 texCoord6	: TEXCOORD6;
	float2 texCoord7	: TEXCOORD7;
	float2 texCoord8	: TEXCOORD8;
	float2 texCoord9	: TEXCOORD9;
};

float4 main(BlurPixelInput input) : SV_TARGET	//replaces worldPos as they are not necessary anymore, and I do not think writing over what i am reading is good practice
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}