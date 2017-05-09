#include "PointLight.h"





PointLight::PointLight(PointLightAttenuation attenuationType) :
	pointLightLayout(),
	diffuseColor(pointLightLayout.diffuse),
	position(pointLightLayout.position),
	radius(pointLightLayout.radius),
	atenConstant(pointLightLayout.attConstant),
	atenLinear(pointLightLayout.attLinear),
	atenQuadratic(pointLightLayout.attQuadratic),
	attenuationType(attenuationType)
	
{
}


PointLight::~PointLight()
{
}

void PointLight::CalcRadiusFromAtten()
{
	// set cutoff back to 0
	pointLightLayout.cutoff = 0;

	//https://learnopengl.com/#!Advanced-Lighting/Deferred-Shading
	// Calculate radius based on attenuation
	float lightMax = XMMax<float>(
		XMMax<float>(diffuseColor.x, diffuseColor.y), diffuseColor.z
		);
	pointLightLayout.radius = (-atenLinear + sqrtf(atenLinear * atenLinear - 4 * atenQuadratic * (atenConstant - (256.0f / 2.0f) * lightMax))) / (2.0f * atenQuadratic);
}

void PointLight::CalcAttenFromRadius()
{
	float lightMax = XMMax<float>(
		XMMax<float>(diffuseColor.x, diffuseColor.y), diffuseColor.z
		);
	float cutoff = lightMax / 4.0f;
	pointLightLayout.attConstant = 1.0f;
	pointLightLayout.attLinear = 2.0f / radius;
	pointLightLayout.attQuadratic = 1.0f / (radius * radius);
	pointLightLayout.cutoff = cutoff * 2;
	//pointLightLayout.cutoff = radius * (sqrtf(lightMax / cutoff) - 1.0f);
}


void PointLight::PrepareForShader()
{
	switch (attenuationType)
	{
		case PointLightAttenuation::CUSTOM:
			break;
		case PointLightAttenuation::ATTEN_FROM_RADIUS:
			CalcAttenFromRadius();
			break;
		case PointLightAttenuation::RADIUS_FROM_ATTEN:
			CalcRadiusFromAtten();
			break;
		default:
			break;
	}

	// Set scale and position for world matrix
	transform.SetPosition(position);
	transform.SetScale(radius, radius, radius);
}
