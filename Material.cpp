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
	pixelShader(pixelShader)
{
	// initialize single texture
	textureList.numTextures = 1;
	textureList.textures = new Texture2D*;
	textureList.textures[0] = texture2D;

	// assigned next ID
	materialID = staticMaterialID++;
}

Material::Material(SimpleVertexShader * const vertexShader,
	SimplePixelShader * const pixelShader,
	Texture2D ** const textures,
	size_t numTextures) :
	vertexShader(vertexShader),
	pixelShader(pixelShader)
{
	// Save multiple textures
	textureList.numTextures = numTextures;
	textureList.textures = new Texture2D*[numTextures];
	
	// copy pointers
	memcpy(textureList.textures, textures, sizeof(Texture2D*) * numTextures);

	// assigned next ID
	materialID = staticMaterialID++;
}

// --------------------------------------------------------
// Destructor
// --------------------------------------------------------
Material::~Material()
{
	// Free double ptr array of textures
	if(textureList.textures)
		delete[] textureList.textures;
}

// --------------------------------------------------------
// Call this from the renderer such that textures and other
// relevant material specific information is set.
// --------------------------------------------------------
void Material::PrepareMaterial()
{
	// Loop through bounds textures and set them as active
	Texture2D* currTexture;
	for (size_t i = textureList.numTextures; i--;)
	{
		currTexture = textureList.textures[i];
		pixelShader->SetShaderResourceView(currTexture->GetSRVName(), currTexture->GetSRV());
		pixelShader->SetSamplerState(currTexture->GetSampelerName(), currTexture->GetSamplerState());

	}
	// This should only be called by the renderer...
	// Set up texture specific information
	//pixelShader->SetShaderResourceView("albedo", texture2D->GetSRV());
	//pixelShader->SetSamplerState("albedoSampler", texture2D->GetSamplerState());

	// Set up additional shader information (if provided)
	// IF I WERE A SPECULAR_MATERIAL
	// SEND UP CAMERA LOCATION
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
	printf("OH NO DONT CALL THIS\n");
	return nullptr;
	//return texture2D;
}

// --------------------------------------------------------
// Get the material ID for this material
// --------------------------------------------------------
unsigned int Material::GetID() const
{
	return materialID;
}


