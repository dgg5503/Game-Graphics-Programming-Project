#pragma once
#include <DirectXMath.h>
#include "DirectionalLightLayout.h"
#include "Light.h"

using namespace DirectX;

class DirectionalLight : private Light
{
	friend class LightRenderer;
public:
	// Exposing struct values publicly
	XMFLOAT4& diffuseColor;
	XMFLOAT4& ambientColor;
	XMFLOAT3& direction;
	float& intensity;

private:
	DirectionalLight();
	~DirectionalLight();

	// Mesh held in LightRenderer
	// PixelShader held in LightRenderer
	// VertexShader held in LighRenderer
	DirectionalLightLayout directionalLightLayout;
};