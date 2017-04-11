


cbuffer ScreenSizeBuffer
{
	float screenWidth;	//I think we have this somewhere
	float3 padding;	//keeps it as a multiple of 16 (float + float3 = 4)
};

// Input info for vertex shader
struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

// Input info to pixel shader
struct BlurPixelInput
{
	float4 position		: SV_POSITION;
	float2 tex			: TEXCOORD0;	//uv coord of origin
	float2 texCoord1	: TEXCOORD1;	//coordinates 4 to the left and 4 to the right to be written to for blur and origin
	float2 texCoord2	: TEXCOORD2;
	float2 texCoord3	: TEXCOORD3;
	float2 texCoord4	: TEXCOORD4;
	float2 texCoord5	: TEXCOORD5;
	float2 texCoord6	: TEXCOORD6;
	float2 texCoord7	: TEXCOORD7;
	float2 texCoord8	: TEXCOORD8;
	float2 texCoord9	: TEXCOORD9;
};

BlurPixelInput main( VertexInput input )
{
	BlurPixelInput output;
	float texelSize = 1.0f / screenWidth;
	output.position = input.position;
	output.tex = input.tex;

	// Create UV coordinates for the pixel and its four horizontal neighbors on either side
	output.texCoord1 = input.tex + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = input.tex + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = input.tex + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = input.tex + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = input.tex + float2(texelSize *  0.0f, 0.0f);
	output.texCoord6 = input.tex + float2(texelSize *  1.0f, 0.0f);
	output.texCoord7 = input.tex + float2(texelSize *  2.0f, 0.0f);
	output.texCoord8 = input.tex + float2(texelSize *  3.0f, 0.0f);
	output.texCoord9 = input.tex + float2(texelSize *  4.0f, 0.0f);

	return output;
}