// Coming from Texture2BufferVShader
struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};

// Create number of buffers equiv to number of buffers in 
Texture2D colorTexture			: register(t0); // COLOR TEXT
Texture2D lightTexture			: register(t1); // LIGHT TEXT
Texture2D emissionTexture		: register(t2); // EMIS TEXT
SamplerState deferredSampler	: register(s0);

/*
	This shader will combine all previous stages of deferred rendering and
	output to the final render target.
*/
float4 main(TargetCoords input) : SV_TARGET
{
	// Sample world pos, normals and emissions
	float4 emission = emissionTexture.Sample(deferredSampler, input.uv);

	//this feels gross for some reason
	if (length(emission.xyz) != 0) {
		return emission;
	}

	float4 col = colorTexture.Sample(deferredSampler, input.uv);
	float4 light = lightTexture.Sample(deferredSampler, input.uv);

	// Combine final output
	return light * col;
}