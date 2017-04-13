#pragma once
#include "Entity.h"
#include "EntityProjectile.h"

class EntityManagerProjectile :
	public Entity
{
public:
	EntityManagerProjectile(EntityFactory* entityFactory, std::string name);
	~EntityManagerProjectile();

	virtual void Update(float deltaTime, float totalTime) override;

	void SetProjectiles(std::vector<EntityProjectile*> projectiles);
	EntityProjectile* GetProjectile();

protected:
	unsigned int numberOfProjectiles;
	unsigned int currentIndex;
	std::vector<EntityProjectile*> projectiles;
};

