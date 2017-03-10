#include "Renderer.h"

// Initialize instance to null
Renderer* Renderer::instance = nullptr;

Renderer::Renderer()
{
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

	/*
	directionalLight[1].DiffuseColor = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	directionalLight[1].Direction = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);
	directionalLight[1].intensity = 0.2f;
	*/
}

Renderer::~Renderer()
{
}


Renderer* const Renderer::Initialize()
{
	// Ensure not already initialized
	assert(instance == nullptr);

	// Initialize renderer
	instance = new Renderer();

	// return instance after init
	return instance;
}

Renderer * const Renderer::Instance()
{
	// Ensure initialized
	assert(instance != nullptr);

	// Return our instance
	return instance;
}

void Renderer::Shutdown()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void Renderer::StageEntity(Entity * const entity)
{
	// add to unordered multimap
	renderBatches.insert(std::make_pair(entity->GetMaterial()->GetID(), entity));
}

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

void Renderer::Render(ID3D11DeviceContext* const context, const Camera * const camera)
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
