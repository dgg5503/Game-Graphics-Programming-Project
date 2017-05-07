#include "DirectionalLight.h"


DirectionalLight::DirectionalLight() :
	directionalLightLayout(),
	diffuseColor(directionalLightLayout.diffuse),
	ambientColor(directionalLightLayout.ambient),
	direction(directionalLightLayout.direction),
	intensity(directionalLightLayout.intensity)
{
	
}

DirectionalLight::~DirectionalLight()
{
}