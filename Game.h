#pragma once

// STL n' stuff
#include <DirectXMath.h>
#include <vector>
#include <map>
#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"

// Camera
#include "Camera.h"
#include "CameraDebug.h"
#include "CameraGame.h"

// Rendering
#include "Renderer.h"
#include "UIRenderer.h"
#include "UIGamePanel.h"

// Entities
#include "Entity.h"
#include "EntityPlayer.h"
#include "TestEntity.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();
	void CreateEntities();

	// Texture stuff
	ID3D11SamplerState* sampler;

	// Renderers
	Renderer* renderer;
	UIRenderer* uiRenderer;

	// Maps of stuff by string
	std::unordered_map<const char*, Mesh*> meshes;
	std::unordered_map<const char*, Texture2D*> textures;
	std::unordered_map<const char*, Material*> materials;
	std::unordered_map<const char*, Entity*> entities;
	std::unordered_map<const char*, UIPanel*> uiPanels;

	// Cameras
	Camera* activeCamera;
	Camera* gameCamera;
	Camera* debugCamera;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader_specular;
	SimplePixelShader* pixelShader;

	

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

