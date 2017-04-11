#include "ProjectileManager.h"


ProjectileManager::ProjectileManager()
{
	currentIndex = 0;;
}


ProjectileManager::~ProjectileManager()
{
}


void ProjectileManager::SetProjectiles(std::vector<EntityProjectile*> projectiles)
{
	this->projectiles = projectiles;
	numberOfProjectiles = projectiles.size();
}

EntityProjectile* ProjectileManager::GetProjectile()
{
	EntityProjectile* projectile = projectiles[currentIndex];
	currentIndex = (currentIndex + 1) % numberOfProjectiles;

	return projectile;
}
