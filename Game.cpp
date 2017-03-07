#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

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
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

	// Initialize renderer singleton
	renderer = Renderer::Initialize();

	// Initialize starting mouse location to center of screen
	prevMousePos.x = width / 2;
	prevMousePos.y = height / 2;

	// Reserve ents
	entities.reserve(16);

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
	for (auto it = entities.begin(); it != entities.end(); it++)
		delete it->second;

	// Free all meshes
	for (auto it = meshes.begin(); it != meshes.end(); it++)
		delete it->second;
	
	// Free all textures
	for (auto it = textures.begin(); it != textures.end(); it++)
		delete it->second;

	// Free all materials
	for (auto it = materials.begin(); it != materials.end(); it++)
		delete it->second;

	// Free sampler state which is being used for all textures
	sampler->Release();

	// Clean up debug camera
	delete debugCamera;

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	// Shutdown renderer
	Renderer::Shutdown();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Create debug camera
	debugCamera = new Camera();
	debugCamera->transform.Move(0, 0, -3.0f);

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these 
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();
	CreateEntities();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

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
	debugCamera->UpdateProjectionMatrix((float)width / height);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create a sampler state
	D3D11_SAMPLER_DESC sampDesc = {}; // inits to all zeros :D!
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // wrap in all dirs
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Trilinear
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &sampler);

	// Texture 1 from http://www.textures.com/download/3dscans0029/126909
	// Texture 2 from http://www.textures.com/download/3dscans0014/126018
	// Load up textures
	textures["brick"] = new Texture2D(L"./Assets/Textures/TexturesCom_BrownBricks_albedo_M.tif",
		sampler,
		device,
		context);
	textures["sand"] = new Texture2D(L"./Assets/Textures/TexturesCom_DesertSand1_albedo_M.tif",
		sampler,
		device,
		context);
	textures["stone"] = new Texture2D(L"./Assets/Textures/TexturesCom_StoneSurface_albedo_M.tif",
		sampler,
		device,
		context);

	// Create our materials
	materials["brick"] = new Material(vertexShader, pixelShader, textures["brick"]);
	materials["sand"] = new Material(vertexShader, pixelShader, textures["sand"]);
	materials["stone"] = new Material(vertexShader, pixelShader, textures["stone"]);

	// Load up all our meshes to a mesh dict
	meshes["cube"] = new Mesh("./Assets/Models/cube.obj", device);
	meshes["helix"] = new Mesh("./Assets/Models/helix.obj", device);
	meshes["torus"] = new Mesh("./Assets/Models/torus.obj", device);
	meshes["cone"] = new Mesh("./Assets/Models/cone.obj", device);
	meshes["cylinder"] = new Mesh("./Assets/Models/cylinder.obj", device);
	meshes["sphere"] = new Mesh("./Assets/Models/sphere.obj", device);
}

// --------------------------------------------------------
// Creates entities using our generated meshes
// --------------------------------------------------------
void Game::CreateEntities()
{
	// Add a new test entity
	entities["bob"] = new TestEntity(meshes["cube"], materials["brick"]);
	entities["coolGuy"] = new TestEntity(meshes["helix"], materials["stone"]);
	entities["torry"] = new TestEntity(meshes["torus"], materials["sand"]);
	entities["sophie"] = new TestEntity(meshes["sphere"], materials["brick"]);
	entities["cynthia"] = new TestEntity(meshes["cylinder"], materials["stone"]);
	entities["sphur"] = new TestEntity(meshes["sphere"], materials["sand"]);

	// Stage all entities for rendering
	for (auto it = entities.begin(); it != entities.end(); it++)
		renderer->StageEntity(it->second);

	// rotate to see if spot light functions?
	entities["cynthia"]->transform.SetRotation(0.0f, 0.0f, 1.0f, XM_PIDIV4);
	entities["cynthia"]->transform.SetPosition(-1.5, 1.5, 0.0f);
	entities["sphur"]->transform.SetScale(0.5f, 2.0f, 3.0f);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update aspect ratio
	debugCamera->UpdateProjectionMatrix((float)width / height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Material change test
	if (GetAsyncKeyState('T') & 0x8000)
	{
		entities["torry"]->SetMaterial(materials["brick"]);
	}

	// set cursor to center of screen
	SetCursorPos(windowLocation.x + width / 2, windowLocation.y + height / 2);

	// Update camera
	debugCamera->Update(deltaTime, totalTime);

	// Update all entities
	/*
	for (size_t i = 0; i < entities.size(); i++)
		entities[i]->Update(deltaTime, totalTime);
	*/

	// Performing individual update here to demonstrate moving entities
	entities["bob"]->transform.SetPosition(XMScalarCos(totalTime), XMScalarSin(totalTime), 0.0f);
	entities["coolGuy"]->transform.SetScale((XMScalarSin(totalTime) + 1) / 2, (XMScalarSin(totalTime) + 1) / 2, (XMScalarSin(totalTime) + 1) / 2);
	entities["torry"]->transform.SetRotation(0.0f, 0.0f, 1.0f, (XMScalarSin(totalTime) + 1) * XM_PI);
	entities["sophie"]->transform.SetPosition(3 * XMScalarCos(totalTime), 0.0f, 3 * XMScalarSin(totalTime));
	
	// ITS SPHERICAL!
	// (spherical coordinates)
	entities["sphur"]->transform.SetPosition(
		3 * XMScalarSin(totalTime) * XMScalarCos(totalTime),
		3 * XMScalarCos(totalTime),
		3 * XMScalarSin(totalTime) * XMScalarSin(totalTime));
	entities["sphur"]->transform.SetRotation(XMScalarCos(totalTime),
		XMScalarSin(totalTime),
		1.0f,
		(XMScalarSin(totalTime / 1000) + 1) * XM_PI);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Render to debug camera
	renderer->Render(context, debugCamera);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
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
	// Add any custom code here...
	// left/right = rotate on X about Y
	// up/down = rotate on Y about Z
	debugCamera->RotateBy(static_cast<float>(x - (width / 2.0f)) / 1000.0f,
		static_cast<float>(y - (height / 2.0f)) / 1000.0f);

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