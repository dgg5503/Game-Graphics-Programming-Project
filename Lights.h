#pragma once
#include <DirectXMath.h>

// --------------------------------------------------------
// Directional Light
//
// Global light that illuminates all objects
// --------------------------------------------------------
struct DirectionalLight
{
	//DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor; // 16
	DirectX::XMFLOAT3 Direction; // 12
	float Intensity; // 4
	//unsigned int : 4; // padding for arrays of lights
};


// --------------------------------------------------------
// Point Light
//
// Light that lives at a point in space, will never dissapate
// --------------------------------------------------------
struct PointLight
{
	DirectX::XMFLOAT4 DiffuseColor; // 16
	DirectX::XMFLOAT3 Position; // 12
	float Intensity; // 4
};


// --------------------------------------------------------
// Spot Light
//
// Light that shines in a cone
// --------------------------------------------------------
struct SpotLight
{
	DirectX::XMFLOAT4 DiffuseColor; // 16
	DirectX::XMFLOAT3 Direction; // 12
	float Intensity; // 4
	DirectX::XMFLOAT3 Position; // 12
	float Angle; // 4
};