// Ignore loop unroll warning
#pragma warning(disable: 3557)

// Light limits
#include "ShaderConstants.h"

// Lighting information to sample from
Texture2D colorTexture			: register(t0);
Texture2D worldPosTexture		: register(t1);
Texture2D normalsTexture		: register(t2);
SamplerState deferredSampler	: register(s0);

// Light data for all lights
// Global information independent of current object
cbuffer LightData : register(b0)
{
	// Matching struct definition from C++ for dirlight
	struct DirectionalLight {
		float4 DiffuseColor; // 16
		float3 Direction; //12 (16)
		float Intensity; // 4
	} directionalLights[MAX_DIR_LIGHTS];

	struct PointLight {
		float4 DiffuseColor; // 16
		float3 Position; // 12 (16)
		float Intensity;
	} pointLights[MAX_POINT_LIGHTS];

	struct SpotLight {
		float4 DiffuseColor; // 16
		float3 Direction; // 16
		float Intensity;
		float3 Position; // 12
		float Angle; // cos(Angle) precalculated
	} spotLights[MAX_SPOT_LIGHTS];

	// global light information
	// Ambient color used for entire scene
	float4 AmbientColor;
};

// Input info from vertex shader
struct TargetCoords
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD;
};

float4 main(TargetCoords input) : SV_TARGET
{
	// Sample color, world pos and normals
	float4 col = colorTexture.Sample(deferredSampler, input.uv);
	float3 pos = worldPosTexture.Sample(deferredSampler, input.uv).xyz;

	// revert normals to -1 to 1
	float3 n = (normalsTexture.Sample(deferredSampler, input.uv).xyz * 2.0f) - 1.0f;

	// Perform lighting calc and return to light texture
	// Resultant light, tmp dir and lightamt
	float4 totalLight = 0;
	float3 dir;
	float lightAmt;
	int i;

	// calc dir light
	for (i = MAX_DIR_LIGHTS; i--;)
	{
		// Calculate direction from object to light
		// This will just be the negated normalized direction of the light since
		// the light is essentially hitting the same spot on the object all the time
		// Negated late for non-negative dotproduct result coming up
		dir = normalize(directionalLights[i].Direction);

		// Calculate how much light is hitting our surface
		// N dot L, normal of surface dot with direction to light from obj
		// clamp amount from 0 to 1 so when angle is > 90, the light amt is
		// 0 instead of negative
		lightAmt = saturate(dot(n, -dir));

		//return directionalLight[i].DiffuseColor * lightAmt;
		totalLight += directionalLights[i].DiffuseColor *
			directionalLights[i].Intensity *
			lightAmt;
	}

	// calc point lights
	for (i = MAX_POINT_LIGHTS; i--;)
	{
		// dir from object to light
		dir = normalize(pointLights[i].Position - pos);

		// normal dot light direction
		lightAmt = saturate(dot(n, dir));

		// add on
		totalLight += pointLights[i].DiffuseColor *
			pointLights[i].Intensity *
			lightAmt;
	}

	// calc spot lights
	for (i = MAX_SPOT_LIGHTS; i--;)
	{
		// dir from object to light
		dir = normalize(spotLights[i].Position - pos);

		// normal dot light direction subtract from angle
		// only accept light from the spotlights direction
		lightAmt = saturate(dot(n, -normalize(spotLights[i].Direction))) *
			saturate(dot(n, dir) - spotLights[i].Angle);

		//lightAmt = max(dot(-dir, normalize(spotLights[i].Direction)), 0.0f);

		// add on
		//totalLight += spotLights[i].DiffuseColor *
		//	pow(lightAmt, spotLights[i].Intensity);
		totalLight += spotLights[i].DiffuseColor *
			spotLights[i].Intensity *
			lightAmt;
	}

	// Scale diffuse by how much light is hitting surface
	// Add in ambient color
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	//return float4(pos, 1.0f);
	return (totalLight + AmbientColor) * col;
}