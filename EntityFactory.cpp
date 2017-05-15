#include "EntityFactory.h"
#include "CollisionManager.h"
#include "MemoryDebug.h"

using namespace std;

Entity* EntityFactory::CreateEntity(EntityType entityType, std::string name, Mesh* mesh, Material* material)
{
	Entity* entity = nullptr;

	switch (entityType)
	{
	case EntityType::STATIC:
		entity = new EntityStatic(this, name, mesh, material);
		break;
	case EntityType::PLAYER:
		entity = new EntityPlayer(this, name, mesh, material);
		break;
	case EntityType::ENEMY:
		entity = new EntityEnemy(this, name, mesh, material);
		break;
	case EntityType::PROJECTILE:
		entity = new EntityProjectile(this, name, mesh, material);
		break;
	case EntityType::MANAGER_PROJECTILE:
		entity = new EntityManagerProjectile(this, name);
		break;
	case EntityType::METEOR:
		entity = new EntityMeteor(this, name, mesh, material);
		break;
	}
	entities[entity->GetName()] = entity;
	return entity;
}

vector<EntityProjectile*> EntityFactory::CreateProjectileEntities(unsigned int numberOfProjectiles, Mesh* mesh, Material* material)
{
	EntityProjectile* projectile;
	vector<EntityProjectile*> projectiles = vector<EntityProjectile*>(numberOfProjectiles);

	for (auto i = 0u; i < numberOfProjectiles; ++i) {
		projectiles[i] = projectile = 
			dynamic_cast<EntityProjectile*>(CreateEntity(EntityType::PROJECTILE, "Projectile_" + std::to_string(i), mesh, material));
		projectile->transform.SetScale(0.15f, 0.15f, 0.15f);
		projectile->transform.SetPosition(0, 0, -200.0f);
		projectile->SetCollider(Collider::SPHERE, XMFLOAT3(0.15f / 2, 0.15f / 2, 0.15f / 2));
		SetEntityCollision(projectile, false);
	}

	return projectiles;
}

void EntityFactory::UpdateEntities(float deltaTime, float totalTime)
{
	for (auto iter = updatingEntities.begin(); iter != updatingEntities.end(); ++iter) {
		iter->second->Update(deltaTime, totalTime);
	}
}

void EntityFactory::SetEntityCollision(Entity* entity, bool isColliding)
{
	if (entity->isColliding == isColliding) {
		return;
	}

	entity->isColliding = isColliding;

	isColliding ? CollisionManager::Instance()->StageCollider(entity->GetCollider()) : CollisionManager::Instance()->UnstageCollider(entity->GetCollider());
}

void EntityFactory::SetEntityRendering(Entity* entity, bool isRendering)
{
	if (entity->isRendering == isRendering) {
		return;
	}
	entity->isRendering = isRendering;

	isRendering ? Renderer::Instance()->StageEntity(entity) : Renderer::Instance()->UnstageEntity(entity);
}

void EntityFactory::SetEntityUpdating(Entity* entity, bool isUpdating)
{
	if (entity->isUpdating == isUpdating) {
		return;
	}
	entity->isUpdating = isUpdating;

	isUpdating ? AddEntityToUpdate(entity) :RemoveEntityFromUpdate(entity);
}

std::unordered_map<std::string, Entity*> EntityFactory::GetEntities()
{
	return entities;
}

void EntityFactory::AddEntityToUpdate(Entity* entity)
{
	updatingEntities[entity->GetName()] = entity;
}

void EntityFactory::RemoveEntityFromUpdate(Entity* entity)
{
	updatingEntities.erase(entity->GetName());
}

void EntityFactory::Release()
{
	// Free all entities
	for (auto iter = entities.begin(); iter != entities.end(); iter++) {
		Entity* entity = iter->second;
		if (entity->isUpdating)
			RemoveEntityFromUpdate(entity);
		if(entity->isColliding)
			CollisionManager::Instance()->UnstageCollider(iter->second->GetCollider());
		if(entity->isRendering)
			Renderer::Instance()->UnstageEntity(iter->second);
		delete entity;
	}
	entities.clear();
}
