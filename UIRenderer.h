#pragma once
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <unordered_map>
#include "UIPanel.h"

using namespace DirectX;

class UIRenderer
{
public:
	UIRenderer();
	~UIRenderer();

	void Initialize(
		ID3D11DeviceContext* const context,	// Current device context for SB and SF
		ID3D11Device* const device			// Current device to load font textures
	);

	// Sets the current panel to draw
	void SetCurrentPanel(UIPanel * panel);

	// Renders a given panel using provided context and device for sprite batch
	void Render();

	// Load a font from a given path onto the GPU
	void LoadFont(const char* const name, const wchar_t * const path);

private:
	// Map by font name
	std::unordered_map<const char*, SpriteFont*> fontMap;

	// Context and device to be used to load font/draw spriteBatch
	ID3D11DeviceContext* context;
	ID3D11Device* device;

	// current panel to draw
	UIPanel* panel;

	// Pointer to single sprite batch which will be used to render all UI
	SpriteBatch* spriteBatch;
};

