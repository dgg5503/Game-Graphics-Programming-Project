#include "Material.h"

// Set material ID start to 0
unsigned int Material::staticMaterialID = 0;

// --------------------------------------------------------
// Constructor
//
// Creates a basic material with supplied vertex and pixel
// shader (required for basic drawing). Also prepares info
// for texture drawing.
// --------------------------------------------------------
Material::Material(SimpleVertexShader * const vertexShader,
	SimplePixelShader * const pixelShader,
	Texture2D* const texture2D) :
	vertexShader(vertexShader),
	pixelShader(pixelShader),
	texture2D(texture2D)
{
	// assigned next ID
	materialID = staticMaterialID++;
}

// --------------------------------------------------------
// Destructor
// --------------------------------------------------------
Material::~Material()
{
}

// --------------------------------------------------------
// Call this from the renderer such that textures and other
// relevant material specific information is set.
// --------------------------------------------------------
void Material::PrepareMaterial()
{
	// This should only be called by the renderer...
	pixelShader->SetShaderResourceView("albedo", texture2D->GetSRV());
	pixelShader->SetSamplerState("albedoSampler", texture2D->GetSamplerState());
}

// --------------------------------------------------------
// Get the vertex shader for this material
// --------------------------------------------------------
SimpleVertexShader * const Material::GetVertexShader() const
{
	return vertexShader;
}

// --------------------------------------------------------
// Get the pixel shader for this material
// --------------------------------------------------------
SimplePixelShader * const Material::GetPixelShader() const
{
	return pixelShader;
}

// --------------------------------------------------------
// Get the texture information for this material
// --------------------------------------------------------
Texture2D * const Material::GetTexture2D() const
{
	return texture2D;
}

// --------------------------------------------------------
// Get the material ID for this material
// --------------------------------------------------------
unsigned int Material::GetID() const
{
	return materialID;
}


