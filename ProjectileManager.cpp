#include "ProjectileManager.h"


ProjectileManager::ProjectileManager()
{
	numberOfProjectiles = 20;
	currentIndex = 0;

	projectiles = std::vector<EntityProjectile*>(numberOfProjectiles);
}


ProjectileManager::~ProjectileManager()
{
}

std::vector<EntityProjectile*> ProjectileManager::BuildProjectiles(Mesh * mesh, Material * material)
{
	EntityProjectile* projectile;
	for (auto i = 0; i < this->numberOfProjectiles; ++i) {
		projectiles[i] = projectile = new EntityProjectile(mesh, material);
		
		projectile->transform.SetScale(0.15f, 0.15f, 0.15f);
		projectile->transform.SetPosition(0, 0, 200.0f);
	}

	return projectiles;
}

EntityProjectile* ProjectileManager::GetProjectile()
{
	EntityProjectile* projectile = projectiles[currentIndex];
	currentIndex = (currentIndex + 1) % projectiles.size();

	return projectile;
}
