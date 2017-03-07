#include "Texture2D.h"

// --------------------------------------------------------
// Constructor
//
// Creates a basic texture from a given filepath and 
// provided sampler state.
// --------------------------------------------------------
Texture2D::Texture2D(const wchar_t * filePath,
	ID3D11SamplerState * const samplerState,
	ID3D11Device * const device,
	ID3D11DeviceContext * const context) :
	samplerState(samplerState)
{
	// Load a texture
	if (DirectX::CreateWICTextureFromFile(
		device,
		context,
		filePath,
		0, // we dont need the texture ref itself
		&srv
	) != S_OK)
		fprintf(stderr, "[Texture2D] Failed to load texture %ls\n", filePath);
}

// --------------------------------------------------------
// Destructor
// --------------------------------------------------------
Texture2D::~Texture2D()
{
	// Free DX11 resources
	// Sampler state actually handled outside of this class!
	//samplerState->Release();
	srv->Release();
}

// --------------------------------------------------------
// Get the sampler state for this texture
// --------------------------------------------------------
ID3D11SamplerState * const Texture2D::GetSamplerState() const
{
	return samplerState;
}

// --------------------------------------------------------
// Get the shader resource view for this texture
// --------------------------------------------------------
ID3D11ShaderResourceView * const Texture2D::GetSRV() const
{
	return srv;
}
