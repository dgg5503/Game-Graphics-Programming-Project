#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Collider.h"
#include "Renderer.h"

class Renderer; // forward declaration to fix cyclic dep
struct Collision {
	Entity* otherEntity;
	Collider* otherCollider;
	Transform otherTransform;
	XMFLOAT3 point;
};

class Entity
{
public:
	// Construction of an entity requires a mesh and material
	Entity(
		Mesh* mesh,
		Material* material
	);
	virtual ~Entity();

	// must be implemented in class which extends this one
	virtual void Update(float deltaTime, float totalTime) = 0;
	//void PrepareMaterial(SimpleVertexShader* const vertexShader);

	virtual void OnCollision(Collision collision);


	// Public transform so we can access information!
	Transform transform;

	// Getters and setters
	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	void SetCollider(Collider::ColliderType type, XMFLOAT3 scale = XMFLOAT3(0, 0, 0), XMFLOAT3 offset = XMFLOAT3(0, 0, 0), XMFLOAT4 rotation = XMFLOAT4(0, 0, 0, 0));
	void SetName(std::string name);
	Mesh * const GetMesh() const;
	Material * const GetMaterial() const;
	Collider * const GetCollider() const;
	std::string GetName() const;

	// Tags
	bool HasTag(std::string tag);
	void AddTag(std::string tag);
	void RemoveTag(std::string tag);


private:
	// Indentifiers
	std::string name;
	std::vector<std::string> tags;

	// Mesh class this entity will draw with
	Mesh* mesh;
	
	// Material containing shaders that this entity will draw with
	Material* material;

	//temp maybe
	Collider* collider;
};

