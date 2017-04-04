// Ignore loop unroll warning
#pragma warning(disable: 3557)

// Light limits
#include "ShaderConstants.h"
#include "Vertex.hlsli"

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

	// used for specular
	float3 CameraLocation;

	// specular power
	float SpecularPower;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Renormalize interpolated normals since it might not be 1 when interpd!
	input.normal = normalize(input.normal);

	// Resultant light, tmp dir and lightamt
	float4 totalLight = 0;
	float3 dir, halfway;
	float lightAmt, specAmt;
	int i;

	// calc dir light
	for (i = MAX_DIR_LIGHTS; i--;)
	{
		// Calculate direction from object to light
		// This will just be the negated normalized direction of the light since
		// the light is essentially hitting the same spot on the object all the time
		// Negated late for non-negative dotproduct result coming up
		dir = normalize(directionalLights[i].Direction);

		// calc halfway between light to obj and eye to obj and spec amt
		halfway = normalize(dir + (input.worldPos - CameraLocation));
		specAmt = pow(saturate(dot(input.normal, halfway)), SpecularPower);

		// Calculate how much light is hitting our surface
		// N dot L, normal of surface dot with direction to light from obj
		// clamp amount from 0 to 1 so when angle is > 90, the light amt is
		// 0 instead of negative
		lightAmt = saturate(dot(input.normal, -dir));

		//return directionalLight[i].DiffuseColor * lightAmt;
		totalLight += directionalLights[i].DiffuseColor *
			directionalLights[i].Intensity *
			specAmt *
			lightAmt;
	}

	// calc point lights
	for (i = MAX_POINT_LIGHTS; i--;)
	{
		// dir from object to light
		dir = normalize(pointLights[i].Position - input.worldPos);

		// calc halfway between light to obj and eye to obj and spec amt
		halfway = normalize(dir + (input.worldPos - CameraLocation));
		specAmt = pow(saturate(dot(input.normal, halfway)), SpecularPower);

		// normal dot light direction
		lightAmt = saturate(dot(input.normal, dir));

		// add on
		totalLight += pointLights[i].DiffuseColor *
			pointLights[i].Intensity *
			specAmt *
			lightAmt;
	}

	// calc spot lights
	for (i = MAX_SPOT_LIGHTS; i--;)
	{
		// dir from object to light
		dir = normalize(spotLights[i].Position - input.worldPos);

		// calc halfway between light to obj and eye to obj and spec amt
		halfway = normalize(dir + (input.worldPos - CameraLocation));
		specAmt = pow(saturate(dot(input.normal, halfway)), SpecularPower);

		// normal dot light direction subtract from angle
		// only accept light from the spotlights direction
		lightAmt = saturate(dot(input.normal, -normalize(spotLights[i].Direction))) *
			saturate(dot(input.normal, dir) - spotLights[i].Angle);

		//lightAmt = max(dot(-dir, normalize(spotLights[i].Direction)), 0.0f);

		// add on
		//totalLight += spotLights[i].DiffuseColor *
		//	pow(lightAmt, spotLights[i].Intensity);
		totalLight += spotLights[i].DiffuseColor *
			spotLights[i].Intensity *
			specAmt *
			lightAmt;
	}

	// Scale diffuse by how much light is hitting surface
	// Add in ambient color
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return totalLight + AmbientColor;

	// Calculate specular amount for point light
	// float3 toCamera = normalize(cameraPosition - input.worldPos);
	// float3 refl = reflect(-dirPL, input.normal);
	// float specular = pow(saturate(dot(refl, toCamera)), 64);
}