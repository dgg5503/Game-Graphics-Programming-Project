Texture2D tex	: register(t0);
SamplerState texSampler	: register(s0);

cbuffer Data : register(b0)
{
	float texelHeight;
	float texelWidth;
};


struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};


struct SizedOutput {
	float4 halfSize	: SV_Target0;
	float4 quarterSize	: SV_Target1;
};

SizedOutput main(TargetCoords input)
{
	SizedOutput output;

	float4 col = float4(0, 0, 0, 0);
	col += tex.Sample(texSampler, (input.uv * 2) + float2(0.0f, -texelHeight));
	col += tex.Sample(texSampler, (input.uv * 2) + float2(-texelWidth, -texelHeight));
	col += tex.Sample(texSampler, (input.uv * 2) + float2(-texelWidth, 0.0f);
	col += tex.Sample(texSampler, (input.uv * 2));
	col = output.quarterSize / 4;
	col.a = 1.0f;
	output.halfSize = col;

	float4 qCol = float4(0, 0, 0, 0);
	qCol += tex.Sample(texSampler, (input.uv * 4) + float2(0.0f, -texelHeight));
	qCol += tex.Sample(texSampler, (input.uv * 4) + float2(-texelWidth, -texelHeight));
	qCol += tex.Sample(texSampler, (input.uv * 4) + float2(-texelWidth, 0.0f);
	qCol += tex.Sample(texSampler, (input.uv * 4));
	qCol = output.quarterSize / 4;
	qCol.a = 1.0f;
	output.quarterSize = qCol;
	//output.quarterSize = tex.Sample(texSampler, input.uv);
	return output;
}