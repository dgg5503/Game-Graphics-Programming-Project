
// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD; // Direction for cubemap sampling in PS
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// Create a translate-less view matrix
	matrix viewNoMovement = view;
	viewNoMovement._41 = 0;
	viewNoMovement._42 = 0;
	viewNoMovement._43 = 0;

	// Calculate output position
	matrix viewProj = mul(viewNoMovement, projection);
	output.position = mul(float4(input.position * 10.0f, 1.0f), viewProj);

	// To ensure we're at MAX DEPTH, set the Z to the W
	output.position.z = output.position.w;

	// Pass through the uvw.  Assuming that, since we're in the center
	// of the cube, the position is also a direction in space
	output.uvw = input.position;

	return output;
}