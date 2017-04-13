//This is a separate shader so that other post processes can be added here without having to go through the blur shaders...maybe
//Could move into vertical blur shader.

Texture2D colorTexture		: register(t0);
Texture2D blurTexture		: register(t1);
SamplerState finalSampler	: register(s0);

struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};

float4 main(TargetCoords input) : SV_TARGET
{
	float4 col = colorTexture.Sample(finalSampler, input.uv);
	float4 blur = blurTexture.Sample(finalSampler, input.uv);
	float4 diff = (col - (col - blur))/2;
	float4 final = col + diff;
	return final;
}