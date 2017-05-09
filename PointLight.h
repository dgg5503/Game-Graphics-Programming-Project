#pragma once

#include "Transform.h"
#include "PointLightLayout.h"
#include "Light.h"

enum class PointLightAttenuation
{
	CUSTOM,
	ATTEN_FROM_RADIUS,
	RADIUS_FROM_ATTEN
};

class PointLight : private Light
{
	friend class LightRenderer;
public:
	// Exposing struct information publicly
	XMFLOAT4& diffuseColor;
	XMFLOAT3& position;
	float& radius;
	float& atenConstant;
	float& atenLinear;
	float& atenQuadratic;

	PointLightAttenuation attenuationType;
private:
	PointLight(PointLightAttenuation attenuationType);
	~PointLight();

	void CalcRadiusFromAtten();
	void CalcAttenFromRadius();
	void PrepareForShader();

	// Mesh held in LightRenderer
	// PixelShader held in LightRenderer
	// VertexShader held in LighRenderer
	Transform transform;
	PointLightLayout pointLightLayout;
};

