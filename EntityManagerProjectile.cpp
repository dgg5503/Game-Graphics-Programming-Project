#include "EntityManagerProjectile.h"
#include "MemoryDebug.h"

EntityManagerProjectile::EntityManagerProjectile(EntityFactory* entityFactory, std::string name) :
	Entity(entityFactory, name)
{
	currentIndex = 0;
	SetIsUpdating(false);
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
