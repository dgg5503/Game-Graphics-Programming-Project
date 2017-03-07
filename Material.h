#pragma once

#include "SimpleShader.h"
#include "Texture2D.h"


class Material
{
public:
	Material(
		SimpleVertexShader* const vertexShader,		// Required shader needed to draw stuff
		SimplePixelShader* const pixelShader,		// Required shader needed to draw stuff
		Texture2D* const texture2D					// Texture to use for this material
	);
	~Material();

	// Set vertex/pixel shader information
	void PrepareMaterial(SimpleVertexShader* const vShader,
		SimplePixelShader* const pShader);

	// Getters for shader types
	SimpleVertexShader* const GetVertexShader() const;
	SimplePixelShader* const GetPixelShader() const;

	// Getters for sampler state and resource view
	Texture2D* const GetTexture2D() const;
	unsigned int GetID() const;

private:
	// Used for unique identification of materials for the Renderer
	static unsigned int staticMaterialID;

	// Pointers to required shaders needed to draw stuff
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Pointers to info neeeded to attach a texture
	Texture2D* texture2D;

	// Material ID for this material
	unsigned int materialID;
};

