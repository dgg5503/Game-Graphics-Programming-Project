#pragma once
#include "EntityProjectile.h"

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	void SetProjectiles(std::vector<EntityProjectile*> projectiles);
	EntityProjectile* GetProjectile();

protected:
	unsigned int numberOfProjectiles;
	unsigned int currentIndex;
	std::vector<EntityProjectile*> projectiles;
};

