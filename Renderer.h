#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <assert.h>
#include <unordered_map>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "UIPanel.h"
#include "Material.h"
#include "Camera.h"
#include "Mesh.h"
#include "Lights.h"
#include "ShaderConstants.h"
#include "Entity.h"
#include "DXWindow.h"

#include "GameState.h"

// defines
#define BUFFER_COUNT 4

// We can include the correct library files here
// instead of in Visual Studio settings if we want
#pragma comment(lib, "d3d11.lib")

class Entity; // forward declaration to fix cyclic dep

class Renderer
{
public:
	// Instance specific stuff
	static Renderer * const Initialize(DXWindow* const window);
	static Renderer * const Instance();
	static void Shutdown();

	// Renderer functions
	void StageEntity(Entity* const entity);
	void UnstageEntity(Entity * const entity);
	void Render(const Camera * const camera);
	void OnResize(unsigned int width, unsigned int height);

	// UI Renderer Specific
	// Sets the current panel to draw
	void SetCurrentPanel(UIPanel * panel);

	// Load a font from a given path onto the GPU
	// TODO: CONVERT TO CREATE FONT AND HANDLE FONTS SOMEWHERE ELSE?
	void LoadFont(const char* const name, const wchar_t * const path);

	// Shader factory. CALLER SHOULD FREE CREATED VARIABLES
	SimpleVertexShader* const CreateSimpleVertexShader() const;
	SimplePixelShader* const CreateSimplePixelShader() const;

	// Mesh factory. CALLER SHOULD FREE CREATED VARIABLES
	Mesh* const CreateMesh(const char* path) const;

	// Texture factory. CALLER SHOULD FREE CREATED VARIABLES
	Texture2D* const CreateTexture2D(const wchar_t * path, Texture2DType type);

	// Get the map of material IDs to entity pointers that are to be rendered
	const std::unordered_multimap<unsigned int, Entity*>& GetRenderBatches() const;

private:
	// Instance specific stuff
	Renderer(DXWindow* const window);
	~Renderer();
	static Renderer* instance;

	// Initialzing DXCORE
	HRESULT InitDirectX(DXWindow* const window);

	// Deferred rendering
	inline void ClearRenderTargets();

	// Rendering UI
	inline void RenderUI();

	// -- DXCORE --
	D3D_FEATURE_LEVEL		dxFeatureLevel;
	IDXGISwapChain*			swapChain;
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

	// -- DEFERRED RENDERING --
	ID3D11Texture2D* depthBufferTexture;
	ID3D11Texture2D* targetTexts[BUFFER_COUNT];
	ID3D11RenderTargetView* targetViews[BUFFER_COUNT];
	ID3D11ShaderResourceView* targetSRVs[BUFFER_COUNT];

	ID3D11Texture2D* lightingTexture;
	ID3D11RenderTargetView* lightingRTV;
	ID3D11ShaderResourceView* lightingSRV;

	ID3D11SamplerState* targetSampler;

	SimpleVertexShader* deferredVS;
	SimplePixelShader* deferredLightingPS;
	SimplePixelShader* deferredCombinePS;


	// -- POSTPROCESSING GLOW --
	ID3D11BlendState* alphaBlendState;

	// -- UI --
	UIPanel* panel;

	// Pointer to single sprite batch which will be used to render all UI
	SpriteBatch* spriteBatch;

	// -- TEXTURE --
	ID3D11SamplerState* sampler;

	// -- LIGHTING --
	// Lights
	DirectionalLight directionalLights[MAX_DIR_LIGHTS] = {};
	PointLight pointLights[MAX_POINT_LIGHTS] = {};
	SpotLight spotLights[MAX_SPOT_LIGHTS] = {};

	// Global lighting information
	DirectX::XMFLOAT4 ambientColor;

	// -- RENDER BATCHES --
	// For every material, render all the meshes!
	// Cool C++11 data structure that allows multiple items to the same key.
	// Exactly what I needed!
	std::unordered_multimap<unsigned int, Entity*> renderBatches;

	// Map by font name
	std::unordered_map<const char*, SpriteFont*> fontMap;

};

