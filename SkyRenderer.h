#pragma once
#include "Renderer.h"


class SkyRenderer
{
	friend class Renderer;

private:
	SkyRenderer(Renderer& renderer);
	~SkyRenderer();

	HRESULT init();
	HRESULT loadShaders();
	HRESULT loadAssets();

	void Render(const Camera * const camera);

	// Reference to renderer which will be used to setup buffers
	Renderer& renderer;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	// -- SKYBOX --
	ID3D11ShaderResourceView* skySRV;
	ID3D11SamplerState* skySampler;
	ID3D11RasterizerState* skyRasterizer;
	ID3D11DepthStencilState* skyDepthStencil;
	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;
	Mesh* skyMesh;
};

