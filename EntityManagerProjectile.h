#pragma once
#include "Entity.h"
#include "EntityProjectile.h"

class EntityManagerProjectile :
	public Entity
{
public:
	EntityManagerProjectile();
	~EntityManagerProjectile();

	void SetProjectiles(std::vector<EntityProjectile*> projectiles);
	EntityProjectile* GetProjectile();

	virtual void Update(float deltaTime, float totalTime) override;

protected:
	unsigned int numberOfProjectiles;
	unsigned int currentIndex;
	std::vector<EntityProjectile*> projectiles;
};

