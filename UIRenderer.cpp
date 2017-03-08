#include "UIRenderer.h"

// --------------------------------------------------------
// Constructor
//
// Initializes fontMap, please use "Initialize(...)" to
// set the device & context and allow for drawing.
// --------------------------------------------------------
UIRenderer::UIRenderer() : panel(nullptr)
{
	// Reserve some space for the map
	fontMap.reserve(16);
	fontMap.rehash(2);
}

// --------------------------------------------------------
// Destructor
// --------------------------------------------------------
UIRenderer::~UIRenderer()
{
	// Free all loaded fonts
	for (auto i = fontMap.begin(); i != fontMap.end(); i++)
		delete i->second;

	// Free spritebatch
	delete spriteBatch;
}

// --------------------------------------------------------
// TO-DO: SOME HOW MERGE WITH REGULAR RENDERER?
//		  IF NOT, AT LEAST MAKE THIS A SINGLETON...
//
// Initializes the UI renderer
//
// context	- current device context needed to create sprite
//			  batch
// device	- current device needed to create sprite fonts
// --------------------------------------------------------
void UIRenderer::Initialize(ID3D11DeviceContext * const context, ID3D11Device * const device)
{
	this->context = context;
	this->device = device;

	// Create sprite batch
	spriteBatch = new SpriteBatch(context);
}

// --------------------------------------------------------
// Sets a single UI panel to be rendered next frame. You
// are expected to encapsulate multiple UI panels within a
// single UI panel since the UI panel is really just a
// container.
//
// panel - The panel to draw next frame
// --------------------------------------------------------
void UIRenderer::SetCurrentPanel(UIPanel* panel)
{
	assert(panel != nullptr);
	this->panel = panel;
}

// --------------------------------------------------------
// Renders the currently set UI panel. Using a single 
// spriteBatch.
// --------------------------------------------------------
void UIRenderer::Render()
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
// Load a font into the font map to be used by panels
//
// name		- name of font, this will be used when grabbing
//			  the font from the font map.
// path		- path to the spritefont file to load
// --------------------------------------------------------
void UIRenderer::LoadFont(const char* const name, const wchar_t * const path)
{
	// Load font at name if it doesnt already exist
	if (!fontMap.count(name))
		fontMap[name] = new SpriteFont(device, path);
	else
		fprintf(stderr, "[UIRenderer] %s already loaded!\n", name);
}
