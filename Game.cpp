#include "Game.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXWindow(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
	pixelShader_normal = 0;
	renderer = nullptr;
	collisionManager = nullptr;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Free all entities
	entityFactory.Release();

	// Free all meshes
	for (auto it = meshes.begin(); it != meshes.end(); it++)
		delete it->second;

	// Free all textures
	for (auto it = textures.begin(); it != textures.end(); it++)
		delete it->second;

	// Free all materials
	for (auto it = materials.begin(); it != materials.end(); it++)
		delete it->second;

	// Free all UI panels
	for (auto it = uiPanels.begin(); it != uiPanels.end(); it++)
		delete it->second;

	// Clean up cameras
	delete debugCamera;
	delete gameCamera;

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete pixelShader_normal;

	// Shutdown renderer
	Renderer::Shutdown();

	// Shutdown Managers
	CollisionManager::Shutdown();
	delete projectileManager;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Initialize renderer singleton/DX
	renderer = Renderer::Initialize(this);
	collisionManager = CollisionManager::Initialize(0.25f, XMFLOAT3(3, 3, 0.5));

	// Initialize starting mouse location to center of screen
	prevMousePos.x = GetWidth() / 2;
	prevMousePos.y = GetHeight() / 2;

	// Create debug camera
	debugCamera = new CameraDebug();
	debugCamera->transform.Move(0, 0, -3.0f);

	gameCamera = new CameraGame();
	gameCamera->transform.Move(0, 0, -100.0f);

	activeCamera = gameCamera;

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these 
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();
	CreateEntities();

	// Load font 
	renderer->LoadFont("arial", L"./Assets/Font/Arial.spritefont");

	// Create a test UI panel
	uiPanels["test"] = new UIGamePanel(0, 0);
	uiPanels["main"] = new UIGamePanel(0, 0);
	uiPanels["game"] = new UIGamePanel(GetWidth()/2, 0);
	uiPanels["end"] = new UIGamePanel(0, 0);

	// Set the panel as current
	renderer->SetCurrentPanel(uiPanels["game"]);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = renderer->CreateSimpleVertexShader();
	if (!vertexShader->LoadShaderFile(L"./Assets/Shaders/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = renderer->CreateSimplePixelShader();
	if (!pixelShader->LoadShaderFile(L"./Assets/Shaders/DefferedPixelShader.cso"))
		pixelShader->LoadShaderFile(L"DefferedPixelShader.cso");

	pixelShader_normal = renderer->CreateSimplePixelShader();
	if (!pixelShader_normal->LoadShaderFile(L"./Assets/Shaders/DefferedPixelShader_NormalMap.cso"))
		pixelShader_normal->LoadShaderFile(L"DefferedPixelShader_NormalMap.cso");

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Setup projection matrix
	debugCamera->UpdateProjectionMatrix((float)GetWidth() / GetHeight());
	gameCamera->UpdateProjectionMatrix((float)GetWidth() / GetHeight());
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Texture 1 from http://www.textures.com/download/3dscans0029/126909
	// Texture 2 from http://www.textures.com/download/3dscans0014/126018
	// Load up textures
	textures["brick"] = renderer->CreateTexture2D(L"./Assets/Textures/TexturesCom_BrownBricks_albedo_M.tif", Texture2DType::ALBEDO);
	textures["sand"] = renderer->CreateTexture2D(L"./Assets/Textures/TexturesCom_DesertSand1_albedo_M.tif", Texture2DType::ALBEDO);
	textures["stone"] = renderer->CreateTexture2D(L"./Assets/Textures/TexturesCom_StoneSurface_albedo_M.tif", Texture2DType::ALBEDO);
	textures["brick_norm"] = renderer->CreateTexture2D(L"./Assets/Textures/TexturesCom_BrownBricks_normalmap_M.tif", Texture2DType::NORMAL);
	textures["no_emission"] = renderer->CreateTexture2D(L"", Texture2DType::EMISSION);

	// Create our materials
	materials["brick"] = new Material(vertexShader, pixelShader_normal, textures["brick"], textures["brick_norm"], textures["no_emission"]);
	materials["sand"] = new Material(vertexShader, pixelShader, textures["sand"]);
	materials["stone"] = new Material(vertexShader, pixelShader, textures["stone"]);

	// Load up all our meshes to a mesh dict
	meshes["cube"] = renderer->CreateMesh("./Assets/Models/cube.obj");
	meshes["helix"] = renderer->CreateMesh("./Assets/Models/helix.obj");
	meshes["torus"] = renderer->CreateMesh("./Assets/Models/torus.obj");
	meshes["cone"] = renderer->CreateMesh("./Assets/Models/cone.obj");
	meshes["cylinder"] = renderer->CreateMesh("./Assets/Models/cylinder.obj");
	meshes["sphere"] = renderer->CreateMesh("./Assets/Models/sphere.obj");
	meshes["enemy"] = renderer->CreateMesh("./Assets/Models/enemyBall.fbx");
	meshes["player"] = renderer->CreateMesh("./Assets/Models/playerShip.fbx");

}

// --------------------------------------------------------
// Creates entities using our generated meshes
// --------------------------------------------------------
void Game::CreateEntities()
{
	entityFactory.SetCollisionManager(collisionManager);

	// Projectile Entities
	projectileManager = new ProjectileManager();
	auto projectiles = entityFactory.CreateProjectileEntities(20, meshes["sphere"], materials["brick"]);
	projectileManager->SetProjectiles(projectiles);


	// Player entity
	EntityPlayer* player;
	//entities["player"] = player = new EntityPlayer(meshes["player"], materials["stone"]);
	player = (EntityPlayer*)entityFactory
		.CreateEntity(ENTITY_TYPE::PLAYER, "player", meshes["player"], materials["stone"]);
	player->SetSpeed(2.0f);
	player->SetSpeed(2.0f);
	player->SetProjectileManager(projectileManager);
	player->transform.SetPosition(0, 0, 0.0f);
	player->transform.SetScale(0.25f, 0.25f, 0.25f);
	player->SetCollider(Collider::ColliderType::SPHERE, XMFLOAT3(0.125f, 0.125f, 0.125f));//sphere mesh is 1 unit in diameter, collider works with radius

	// leaks here
	// Enemy entities
	EntityEnemy* enemy;
	for (auto i = 0u; i < 5; ++i) {
		auto name = new std::string("Enemy_" + std::to_string(i));
		//entities[name->data()] = enemy = new EntityEnemy(meshes["enemy"], materials["sand"]);
		enemy = (EntityEnemy*)entityFactory
			.CreateEntity(ENTITY_TYPE::ENEMY, "Enemy_" + std::to_string(i), meshes["enemy"], materials["sand"]);
		enemy->SetTarget(player);
		enemy->MoveToRandomPosition();
		enemy->transform.SetScale(0.25f, 0.25f, 0.25f);
		enemy->SetCollider(Collider::ColliderType::OBB);
	}

	// Background entity
	Entity* background = entityFactory
		.CreateEntity(ENTITY_TYPE::STATIC, "Background", meshes["cube"], materials["brick"]);
	background->transform.SetPosition(0, 0, 5.0f);
	background->transform.SetScale(8.0f, 5.0f, 1.0f);

	// Stage all entities for rendering
	auto entities = entityFactory.GetEntities();
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Stage renderer
		if (it->second->isRendering) {
		renderer->StageEntity(it->second);
		}
		// Stage Colliders
		if (it->second->isColliding) {
			collisionManager->StageCollider(it->second->GetCollider());
		}
	}
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize(unsigned int width, unsigned int height)
{
	// Quick temp fix for checking if game is initialized
	if (renderer)
	{
		// Resize renderer outputs
		renderer->OnResize(width, height);

		// Update aspect ratio
		debugCamera->UpdateProjectionMatrix((float)width / height);
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	if (GetAsyncKeyState('1') & 0x8000)
	{
		activeCamera = gameCamera;
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		activeCamera = debugCamera;
	}


	//ui panels text updating
	timerString = std::to_wstring(minutes) + L": " + std::to_wstring(seconds) + L": " + std::to_wstring(milliseconds);

	milliseconds += deltaTime * 1000;
	if (milliseconds >= 1000) {
		seconds++;
		milliseconds = 0;
	}
	if (seconds >= 60) {
		minutes++;
		seconds = 0;
	}
	uiPanels["game"]->UpdateText(timerString);
	//


	//mouse pos
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	mouseX = cursorPos.x;
	mouseY = cursorPos.y;


	// Update camera
	activeCamera->Update(deltaTime, totalTime);

	// Update all entities
	entityFactory.UpdateEntities(deltaTime, totalTime);
	
	// set cursor to center of screen
	if (activeCamera == debugCamera) {
		SetCursorPos(
			GetWindowLocation().x + GetWidth() / 2,
			GetWindowLocation().y + GetHeight() / 2
		);
	}


	//check for collisions
	collisionManager->CollisionUpdate();

	// Dispatch compute shaders
	renderer->UpdateCS(deltaTime, totalTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Render to active camera
	renderer->Render(activeCamera);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// If the debug camera is active.
	if (activeCamera == debugCamera)
	{
		// left/right = rotate on X about Y
		// up/down = rotate on Y about Z
		debugCamera->RotateBy(static_cast<float>(x - (GetWidth() / 2.0f)) / 1000.0f,
			static_cast<float>(y - (GetHeight() / 2.0f)) / 1000.0f);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion