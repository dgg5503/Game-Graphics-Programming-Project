#include "EntityManagerProjectile.h"



EntityManagerProjectile::EntityManagerProjectile() : Entity()
{
	currentIndex = 0;
	isUpdating = false;
}


EntityManagerProjectile::~EntityManagerProjectile()
{
}

void EntityManagerProjectile::SetProjectiles(std::vector<EntityProjectile*> projectiles)
{
	this->projectiles = projectiles;
	numberOfProjectiles = projectiles.size();
}

EntityProjectile* EntityManagerProjectile::GetProjectile()
{
	EntityProjectile* projectile = projectiles[currentIndex];
	currentIndex = (currentIndex + 1) % numberOfProjectiles;

	return projectile;
}

void EntityManagerProjectile::Update(float deltaTime, float totalTime)
{
}
