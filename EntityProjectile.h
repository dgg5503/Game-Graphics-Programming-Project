#pragma once
#include "Entity.h"

class ProjectileManager;

class EntityProjectile :
	public virtual Entity
{
public:
	EntityProjectile(Mesh* mesh, Material* material);
	~EntityProjectile();

	void Update(float deltaTime, float totalTime) override;

	// Remove the bullet from the field (Moving it out of frame)
	void Remove();

	// Getters and Setters
	void SetSpeed(float speed);
	float GetSpeed();
	void SetDirection(XMFLOAT3 direction);
	XMFLOAT3* GetDirection();

protected:
	ProjectileManager* manager;	// Pointer to manager

	float speed;
	XMFLOAT3 direction;

	void OnCollision(Collision collison) override;
};

