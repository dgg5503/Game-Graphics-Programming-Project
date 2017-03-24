// Texture information for objects
Texture2D albedo			: register(t0);
SamplerState albedoSampler	: register(s0);

// Struct representing the data we're sending down the pipeline
// - Should match our pixel shader's input (hence the name: Vertex to Pixel)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position 
	float3 worldPos		: WORLDPOSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

// Output our pixel data to render targets
struct PSOutput
{
	float4 color		: SV_Target0;
	float4 worldPos		: SV_Target1;
	float4 normals		: SV_Target2;
};

// Output all of our data to render targets (screen sized textures)
PSOutput main(VertexToPixel input) : SV_TARGET
{
	// Redirect interpolated pixels to render targets
	PSOutput output;
	
	// sample color information
	output.color = albedo.Sample(albedoSampler, input.uv);

	// sample world pos
	output.worldPos = float4(input.worldPos, 1.0f);

	// sample normals
	output.normals = float4(input.normal, 1.0f);

	// return to render targets
	return output;
}