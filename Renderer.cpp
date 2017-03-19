#include "Renderer.h"

// Initialize instance to null
Renderer* Renderer::instance = nullptr;

Renderer::Renderer(DXWindow* const window)
{
	HRESULT ret;

	// Init DXCore
	ret = InitDirectX(window);
	if (ret != S_OK)
		fprintf(stderr, "[Renderer] Failed to initialize DXCore\n");

	// Load assets

	// Set initial number of buckets and stuff
	renderBatches.rehash(16);
	renderBatches.reserve(16);

	// Init lights
	// Make sure to change the MAX_LIGHT defines in ShaderConstants.h if you
	// want more lights!
	ambientColor = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLights[0].DiffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLights[0].Direction = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	directionalLights[0].Intensity = 1.0f;

	pointLights[0].DiffuseColor = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pointLights[0].Position = DirectX::XMFLOAT3(0, 0, 0);
	pointLights[0].Intensity = 1.0f;

	spotLights[0].DiffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spotLights[0].Position = DirectX::XMFLOAT3(-2.0f, 2.0f, 0.0f);
	spotLights[0].Direction = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	spotLights[0].Angle = DirectX::XMScalarCos(XM_PIDIV4);
	spotLights[0].Intensity = 1.0f;

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Setup texture stuff
	// Create a sampler state
	D3D11_SAMPLER_DESC sampDesc = {}; // inits to all zeros :D!
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // wrap in all dirs
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Trilinear
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, &sampler);

	// Initialize UI stuff
	spriteBatch = new SpriteBatch(context);
	panel = nullptr;

	//temporary
	gameState = MAIN_MENU;

	/*
	directionalLight[1].DiffuseColor = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	directionalLight[1].Direction = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);
	directionalLight[1].intensity = 0.2f;
	*/
}

// --------------------------------------------------------
// Destructor - Clean up (release) all DirectX references
// --------------------------------------------------------
Renderer::~Renderer()
{
	// Free sampler state which is being used for all textures
	sampler->Release();
}

// --------------------------------------------------------
// Initializes DirectX, which requires a window.  This method
// also creates several DirectX objects we'll need to start
// drawing things to the screen.
// TO-DO: COMMENT REST OF FUNCTIONS
//
// window - reference to window to draw to
// --------------------------------------------------------
HRESULT Renderer::InitDirectX(DXWindow* const window)
{
	// This will hold options for DirectX initialization
	unsigned int deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// If we're in debug mode in visual studio, we also
	// want to make a "Debug DirectX Device" to see some
	// errors and warnings in Visual Studio's output window
	// when things go wrong!
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create a description of how our swap
	// chain should work
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = window->GetWidth();
	swapDesc.BufferDesc.Height = window->GetHeight();
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = 0;
	swapDesc.OutputWindow = window->GetWindowReference();
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;

	// Result variable for below function calls
	HRESULT hr = S_OK;

	// Attempt to initialize DirectX
	hr = D3D11CreateDeviceAndSwapChain(
		0,							// Video adapter (physical GPU) to use, or null for default
		D3D_DRIVER_TYPE_HARDWARE,	// We want to use the hardware (GPU)
		0,							// Used when doing software rendering
		deviceFlags,				// Any special options
		0,							// Optional array of possible verisons we want as fallbacks
		0,							// The number of fallbacks in the above param
		D3D11_SDK_VERSION,			// Current version of the SDK
		&swapDesc,					// Address of swap chain options
		&swapChain,					// Pointer to our Swap Chain pointer
		&device,					// Pointer to our Device pointer
		&dxFeatureLevel,			// This will hold the actual feature level the app will use
		&context);					// Pointer to our Device Context pointer
	if (FAILED(hr)) return hr;

	// The above function created the back buffer render target
	// for us, but we need a reference to it
	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backBufferTexture);

	// Now that we have the texture, create a render target view
	// for the back buffer so we can render into it.  Then release
	// our local reference to the texture, since we have the view.
	device->CreateRenderTargetView(
		backBufferTexture,
		0,
		&backBufferRTV);
	backBufferTexture->Release();

	// Set up the description of the texture to use for the depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = window->GetWidth();
	depthStencilDesc.Height = window->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view, then 
	// release our reference to the texture
	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();

	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	// Lastly, set up a viewport so we render into
	// to correct portion of the window
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)window->GetWidth();
	viewport.Height = (float)window->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	// Return the "everything is ok" HRESULT value
	return S_OK;
}

// --------------------------------------------------------
// Renders the currently set UI panel. Using a single 
// spriteBatch.
// --------------------------------------------------------
inline void Renderer::RenderUI()
{
	assert(panel != nullptr);

	// Start sprite batch
	spriteBatch->Begin();

	// draw panel
	panel->Draw(spriteBatch, fontMap);

	// Stop sprite batch
	spriteBatch->End();
}

// --------------------------------------------------------
// Initialize the renderer by attaching it to a window.
// --------------------------------------------------------
Renderer* const Renderer::Initialize(DXWindow* const window)
{
	// Ensure not already initialized
	assert(instance == nullptr);

	// Initialize renderer
	instance = new Renderer(window);

	// return instance after init
	return instance;
}

// --------------------------------------------------------
// Get the current instance of this renderer
// --------------------------------------------------------
Renderer * const Renderer::Instance()
{
	// Ensure initialized
	assert(instance != nullptr);

	// Return our instance
	return instance;
}

// --------------------------------------------------------
// Shutdown the renderer instance
// --------------------------------------------------------
void Renderer::Shutdown()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

// --------------------------------------------------------
// Stage an entity to be rendered.
// NOTE: Currently need to unstage and restage entity when
// material is modified/changed. This is already done in
// Entity->SetMaterial() for you.
//
// entity - the entity to render
// --------------------------------------------------------
void Renderer::StageEntity(Entity * const entity)
{
	// add to unordered multimap
	renderBatches.insert(std::make_pair(entity->GetMaterial()->GetID(), entity));
}


// --------------------------------------------------------
// Removes an entity from its render batch. This will cause
// it to not render.
// NOTE: Currently need to unstage and restage entity when
// material is modified/changed. This is already done in
// Entity->SetMaterial() for you.
//
// entity - the entity to remove from the render batch
// --------------------------------------------------------
void Renderer::UnstageEntity(Entity * const entity)
{
	// grab bin
	auto bucket = renderBatches.equal_range(entity->GetMaterial()->GetID());
	auto iterator = bucket.first;

	// loop until we find the entity and remove
	// WARNING: ENTITY POINTER SHOULD NEVER EVER CHANGE SINCE THE START OF
	// THE PROGRAM. DO NOT MOVE THE POINTER!
	for (; iterator != bucket.second; iterator++)
	{
		if (iterator->second == entity)
		{
			renderBatches.erase(iterator);
			break;
		}
	}
}

// --------------------------------------------------------
// Renders currently staged objects to a given camera
//
// camera - view point to use when rendering objects
// --------------------------------------------------------
void Renderer::Render(const Camera * const camera)
{
	if (gameState == GAME)
	{
		// Shaders we will work with for each bucket
		SimpleVertexShader* vertexShader;
		SimplePixelShader* pixelShader;

		// Current vert buffer we're drawing
		const Mesh* currMesh;
		Material* currMaterial;
		Entity* currEntity;
		ID3D11Buffer* currVertBuff;

		// Camera information that will not change mid-render
		XMFLOAT4X4 view = camera->GetViewMatrix();
		XMFLOAT4X4 projection = camera->GetProjectionMatrix();

		// Background color (Cornflower Blue in this case) for clearing
		const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		// Clear the render target and depth buffer (erases what's on the screen)
		//  - Do this ONCE PER FRAME
		//  - At the beginning of Draw (before drawing *anything*)
		context->ClearRenderTargetView(backBufferRTV, color);
		context->ClearDepthStencilView(
			depthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0);

		// Iterate through each bucket
		for (auto it = renderBatches.begin(); it != renderBatches.end();)
		{
			auto bucket = renderBatches.equal_range(it->first);

			// Grab current shaders to work with
			currMaterial = bucket.first->second->GetMaterial();
			vertexShader = currMaterial->GetVertexShader();
			pixelShader = currMaterial->GetPixelShader();

			// -- Camera --
			vertexShader->SetMatrix4x4("view", view);
			vertexShader->SetMatrix4x4("projection", projection);

			// -- Lighting --
			pixelShader->SetDataAligned(
				"directionalLights",						// name of variable in ps
				&directionalLights,							// direction of light
				sizeof(DirectionalLight) * MAX_DIR_LIGHTS	// size of struct * maxdirlights
			);

			pixelShader->SetDataAligned(
				"pointLights",								// name of variable in ps
				&pointLights,								// direction of light
				sizeof(PointLight) * MAX_POINT_LIGHTS		// size of struct * maxdirlights
			);

			pixelShader->SetDataAligned(
				"spotLights",								// name of variable in ps
				&spotLights,							 	// direction of light
				sizeof(SpotLight) * MAX_SPOT_LIGHTS		    // size of struct * maxdirlights
			);
			pixelShader->SetFloat4("AmbientColor", ambientColor);

			// -- Set material specific information --
			currMaterial->PrepareMaterial();

			// -- Copy pixel data --
			pixelShader->CopyAllBufferData();

			for (auto bucketIt = bucket.first; bucketIt != bucket.second; bucketIt++)
			{
				// -- Set material specific information --

				// How to pass in cameralocation for a blinn-phone material
				// when I can only supply the vertexShader and pixelShader?

				// The camera location is something that sits constant during these calcs
				// We need to make a function in material that prepares constant information
				// for the given shader?

				// -- Grab current entity --
				currEntity = bucketIt->second;

				// -- Set entity specific info --
				// below exist for every entity.
				vertexShader->SetMatrix4x4("world",
					currEntity->transform.GetWorldMatrix());
				vertexShader->SetMatrix4x4("inverseTransposeWorld",
					currEntity->transform.GetInverseTransposeWorldMatrix());

				// -- Copy vertex data --
				vertexShader->CopyAllBufferData();

				// -- Set shaders --
				vertexShader->SetShader();
				pixelShader->SetShader();

				// -- Draw model --
				// Set buffers in the input assembler
				//  - Do this ONCE PER OBJECT you're drawing, since each object might
				//    have different geometry.
				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				currMesh = bucketIt->second->GetMesh();
				currVertBuff = currMesh->GetVertexBuffer();
				context->IASetVertexBuffers(0, 1, &currVertBuff, &stride, &offset);
				context->IASetIndexBuffer(currMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

				// Finally do the actual drawing
				//  - Do this ONCE PER OBJECT you intend to draw
				//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
				//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
				//     vertices in the currently set VERTEX BUFFER
				context->DrawIndexed(
					currMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
					0,     // Offset to the first index we want to use
					0);    // Offset to add to each index when looking up vertices
			}

			// move to next bucket
			it = bucket.second;
		}
	}

	// Render UI
	RenderUI();

	// Fixes depth buffer issue
	context->OMSetDepthStencilState(nullptr, 0);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}

// --------------------------------------------------------
// When the window is resized, the underlying 
// buffers (textures) must also be resized to match.
//
// If we don't do this, the window size and our rendering
// resolution won't match up.  This can result in odd
// stretching/skewing.
//
// width - new width to resize to
// height - new height to resize to
// --------------------------------------------------------
void Renderer::OnResize(unsigned int width, unsigned int height)
{
	// Release existing DirectX views and buffers
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBufferRTV) { backBufferRTV->Release(); }

	// Resize the underlying swap chain buffers
	swapChain->ResizeBuffers(
		1,
		width,
		height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);

	// Recreate the render target view for the back buffer
	// texture, then release our local texture reference
	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));
	device->CreateRenderTargetView(backBufferTexture, 0, &backBufferRTV);
	backBufferTexture->Release();

	// Set up the description of the texture to use for the depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view, then 
	// release our reference to the texture
	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();

	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	// Lastly, set up a viewport so we render into
	// to correct portion of the window
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);
}

// --------------------------------------------------------
// Sets a single UI panel to be rendered next frame. You
// are expected to encapsulate multiple UI panels within a
// single UI panel since the UI panel is really just a
// container.
//
// panel - The panel to draw next frame
// --------------------------------------------------------
void Renderer::SetCurrentPanel(UIPanel * panel)
{
	assert(panel != nullptr);
	this->panel = panel;
}

// --------------------------------------------------------
// Load a font into the font map to be used by panels
//
// name		- name of font, this will be used when grabbing
//			  the font from the font map.
// path		- path to the spritefont file to load
// --------------------------------------------------------
void Renderer::LoadFont(const char * const name, const wchar_t * const path)
{
	// Load font at name if it doesnt already exist
	if (!fontMap.count(name))
		fontMap[name] = new SpriteFont(device, path);
	else
		fprintf(stderr, "[UIRenderer] %s already loaded!\n", name);
}

SimpleVertexShader * const Renderer::CreateSimpleVertexShader() const
{
	return new SimpleVertexShader(device, context);
}

SimplePixelShader * const Renderer::CreateSimplePixelShader() const
{
	return new SimplePixelShader(device, context);
}

Mesh * const Renderer::CreateMesh(const char * path) const
{
	if (!path)
		return nullptr;
	return new Mesh(path, device);
}

Texture2D * const Renderer::CreateTexture2D(const wchar_t * path)
{
	if (!path)
		return nullptr;
	return new Texture2D(path, sampler, device, context);
}
