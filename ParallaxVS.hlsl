#include "Vertex.hlsli"

cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	matrix inverseTransposeWorld;
};

struct PVStoPS
{
	float4 position : SV_Position;
	float3 worldPos : POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
	float3 viewTan		: VIEWTANGENT;
	float3 posTan		: POSITIONTANGENT;
};

PVStoPS main(VertexShaderInput input)
{
	PVStoPS output;

	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = (float3) mul(float4(input.position, 1.0f), world);

	output.normal = mul(input.normal, (float3x3)inverseTransposeWorld);
	output.tangent = mul(input.tangent, (float3x3)inverseTransposeWorld);

	/*
	float3 T = normalize(mul(input.tangent, (float3x3)world));
	float3 B = normalize(mul(input.bitangent, (float3x3)world));
	float3 N = normalize(mul(input.normal, (float3x3)world));
	float3x3 TBN = transpose(float3x3(T, B, N));//This right?
	*/

	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.normal, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	//output.viewTan = mul(viewPos, TBN);//How to get viewPos?
	output.posTan = mul(output.position, TBN);

	// Interpolate UV coordinates
	output.uv = input.uv;

	return output;
}