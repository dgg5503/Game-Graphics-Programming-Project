#pragma once
#include "Material.h"
class MaterialEnemy :
	public Material
{
public:
	MaterialEnemy(
		SimpleVertexShader * const vertexShader,	// Required shader needed to draw stuff
		SimplePixelShader * const pixelShader,		// Required shader needed to draw stuff
		Texture2D * const albedoTexture,			// albedo texture
		Texture2D * const normalTexture,			// normal texture
		Texture2D * const emissionTexture			// emission texture
		);
	~MaterialEnemy();

	void PrepareMaterial() override;

	void SetTotalTime(float totalTime);

private:
	float totalTime, timeOffset;
};

