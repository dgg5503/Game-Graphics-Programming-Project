Texture2D tex	: register(t0);
SamplerState texSampler	: register(s0);


struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};


struct SizedOutput {
	float4 quarterSize	: SV_Target0;
};

SizedOutput main(TargetCoords input)
{
	SizedOutput output;
	float texelHeight = 1 / 720;
	float texelWidth = 1 / 1280;
	float4 col = float4(0, 0, 0, 0);
	//doesn't output a texture downsized, only the top left corner of the full texture --> TODO: output full texture downsized --> method?
	//0,0 is drawing 0,0 and pixel to right and down
	// 0.5,0.5 is drawing 1,1
	//all must shift over by a multiple of some value so that the whole tex is sampled for the smaller one? Or is it that this is not sampling the correct values <-- this i think
	col += tex.Sample(texSampler, input.uv + float2(0.0f, texelHeight));
	col += tex.Sample(texSampler, input.uv + float2(texelWidth, 0.0f));
	col += tex.Sample(texSampler, input.uv);
	col = output.quarterSize / 3;
	col.a = 1.0f;
	output.quarterSize = col;
	//output.quarterSize = tex.Sample(texSampler, input.uv);
	return output;
}