#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <assert.h>
#include <unordered_map>
#include "Material.h"
#include "Camera.h"
#include "Mesh.h"
#include "Lights.h"
#include "ShaderConstants.h"
#include "Entity.h"

class Entity; // forward declaration to fix cyclic dep

class Renderer
{
public:
	// Instance specific stuff
	static Renderer * const Initialize();
	static Renderer * const Instance();
	static void Shutdown();


	// Renderer functions
	void StageEntity(Entity* const entity);
	void UnstageEntity(Entity * const entity);
	void Render(ID3D11DeviceContext* const context, const Camera * const camera);

private:
	// Instance specific stuff
	Renderer();
	~Renderer();
	static Renderer* instance;

	// Renderer stuff
	// Lights
	DirectionalLight directionalLights[MAX_DIR_LIGHTS] = {};
	PointLight pointLights[MAX_POINT_LIGHTS] = {};
	SpotLight spotLights[MAX_SPOT_LIGHTS] = {};

	// Global lighting information
	DirectX::XMFLOAT4 ambientColor;

	// For every material, render all the meshes!
	// Cool C++11 data structure that allows multiple items to the same key.
	// Exactly what I needed!
	std::unordered_multimap<unsigned int, Entity*> renderBatches;

};

