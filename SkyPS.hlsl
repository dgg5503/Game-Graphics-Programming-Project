
// External texture-related data
TextureCube Skybox		: register(t0); // Not a Texture2D!
SamplerState Sampler	: register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD;
};


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	return Skybox.Sample(Sampler, input.uvw);
}