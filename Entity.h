#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

class Entity
{
public:
	// Cosntruction of an entity requires a mesh and material
	Entity(
		Mesh* mesh,
		Material* material
	);
	virtual ~Entity();

	// must be implemented in class which extends this one
	virtual void Update(float deltaTime, float totalTime) = 0;
	//void PrepareMaterial(SimpleVertexShader* const vertexShader);

	// Getters and setters
	void SetMesh(Mesh* mesh);
	Mesh * const GetMesh() const;
	Material * const GetMaterial() const;


	// Public transform so we can access information!
	Transform transform;

private:
	// Mesh class this entity will draw with
	Mesh* mesh;
	
	// Material containing shaders that this entity will draw with
	Material* material;
};

