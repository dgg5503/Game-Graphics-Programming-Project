#pragma once

// STL n' stuff
#include <DirectXMath.h>
#include <vector>
#include <map>
#include "DXWindow.h"
#include "SimpleShader.h"
#include "Mesh.h"

// Camera
#include "Camera.h"
#include "CameraDebug.h"
#include "CameraGame.h"

// Rendering
#include "Renderer.h"
#include "UIGamePanel.h"

//Collisions
#include "CollisionManager.h"

// Entities
#include "EntityFactory.h"

// Scenes/States
#include "StateManager.h"
#include "Scene.h"
#include "SceneGame.h"
#include "SceneMenu.h"


class Game 
	: public DXWindow
{
public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize(unsigned int width, unsigned int height);
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

	// Entities
	EntityFactory entityFactory;

	// Renderers
	Renderer* renderer;

	//Collision Manager
	CollisionManager* collisionManager;

	// Maps of stuff by string
	std::unordered_map<const char*, Mesh*> meshes;
	std::unordered_map<const char*, Texture2D*> textures;
	std::unordered_map<const char*, Material*> materials;

	// State Manager;
	StateManager stateManager;

	// Cameras
	Camera* activeCamera;
	Camera* gameCamera;
	Camera* debugCamera;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* pixelShader_normal;
	SimpleVertexShader* vertexShader_enemy;
	SimpleVertexShader* vertexShader_parallax;
	SimplePixelShader* pixelShader_parallax;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//mouse x and y positions
	float mouseY;
	float mouseX;
};

