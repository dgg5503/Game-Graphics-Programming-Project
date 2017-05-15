#pragma once
#include "Mesh.h"
#include "Material.h"
#include "MaterialParallax.h"
#include "Transform.h"
#include "Collider.h"
#include "Renderer.h"

class Renderer; 
class EntityFactory;

struct Collision {
	Entity* otherEntity;
	Collider* otherCollider;
	Transform otherTransform;
	XMFLOAT3 point;
};

class Entity
{
	friend EntityFactory;

public:
	// Construction of an entity requires a mesh and material
	Entity(
		EntityFactory* entityFactory,
		std::string name,
		Mesh* mesh = nullptr,
		Material* material = nullptr
	);
	virtual ~Entity();

	// must be implemented in class which extends this one
	virtual void Update(float deltaTime, float totalTime) = 0;
	//void PrepareMaterial(SimpleVertexShader* const vertexShader);

	virtual void OnCollision(Collision collision);

	// Public transform so we can access information!
	Transform transform;

	// Getters and Setters
	void SetEntityFactory(EntityFactory* entityFactory);

	void SetIsUpdating(bool isUpdating);
	void SetIsRendering(bool isRendering);
	void SetIsColliding(bool isColliding);

	bool GetIsUpdating();
	bool GetIsRendering();
	bool GetIsColliding();

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
	EntityFactory* entityFactory;

	// Properties
	bool isUpdating = false;
	bool isRendering = false;
	bool isColliding = false;

	// Identifiers
	std::string name;
	std::vector<std::string> tags;

	// Mesh class this entity will draw with
	Mesh* mesh = nullptr;
	
	// Material containing shaders that this entity will draw with
	Material* material = nullptr;

	//temp maybe
	Collider* collider = nullptr;
};

