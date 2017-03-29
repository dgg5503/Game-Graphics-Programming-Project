#pragma once
#include "Entity.h"
class EntityProjectile :
	public virtual Entity
{
public:
	EntityProjectile(Mesh* mesh, Material* material);
	~EntityProjectile();

	// Set new update
	void Update(float deltaTime, float totalTime) override;

	void SetSpeed(float speed);
	float GetSpeed();

	void SetDirection(XMFLOAT3 direction);
	XMFLOAT3* GetDirection();

protected:
	float speed;
	XMFLOAT3 direction;
};

