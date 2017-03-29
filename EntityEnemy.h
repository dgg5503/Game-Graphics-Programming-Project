#pragma once
#include "Entity.h"

class EntityEnemy :
	virtual public Entity
{
public:
	EntityEnemy(Mesh* mesh, Material* material);
	~EntityEnemy();

	// Set new update
	void Update(float deltaTime, float totalTime) override;

	void SetSpeed(float speed);
	float GetSpeed();

	void SetHealth(float health);
	float GetHealth();

	void SetMaxHealth(float healthMax);
	float GetMaxHealth();

	void SetTarget(Entity* target);
	Entity* GetTarget();

	void SetDirection(XMFLOAT3 direction);
	const XMFLOAT3* const GetDirection() const;

protected:
	float speed;
	float health;
	float healthMax;

	Entity* target;
	XMFLOAT3 direction;

	void OnCollision(Collision collision) override;
};

