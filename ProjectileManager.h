#pragma once
#include "EntityProjectile.h"

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	std::vector<EntityProjectile*> BuildProjectiles(Mesh* mesh, Material* material);
	EntityProjectile* GetProjectile();

protected:
	unsigned int numberOfProjectiles;
	unsigned int currentIndex;
	std::vector<EntityProjectile*> projectiles;
};

