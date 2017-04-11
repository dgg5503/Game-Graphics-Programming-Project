#include "EntityFactory.h"
#include "CollisionManager.h"

using namespace std;

Entity* EntityFactory::CreateEntity(ENTITY_TYPE entityType, std::string name, Mesh* mesh, Material* material)
{
	Entity* entity = nullptr;

	switch (entityType)
	{
	case ENTITY_TYPE::STATIC:
		entity = new EntityStatic(mesh, material);
		break;
	case ENTITY_TYPE::PLAYER:
		entity = new EntityPlayer(mesh, material);
		break;
	case ENTITY_TYPE::ENEMY:
		entity = new EntityEnemy(mesh, material);
		break;
	case ENTITY_TYPE::PROJECTILE:
		entity = new EntityProjectile(mesh, material);
		break;
	}

	entity->SetName(name);
	entity->SetEntityFactory(this);
	entities[name.data()] = entity;

	if (entity->isUpdating) {
		AddEntityToUpdate(entity);
	}
	if (entity->isRendering) {
		renderer->StageEntity(entity);
	}

	return entity;
}

vector<EntityProjectile*> EntityFactory::CreateProjectileEntities(unsigned int numberOfProjectiles, Mesh* mesh, Material* material)
{
	EntityProjectile* projectile;
	vector<EntityProjectile*> projectiles = vector<EntityProjectile*>(numberOfProjectiles);

	for (auto i = 0u; i < numberOfProjectiles; ++i) {
		projectiles[i] = projectile = 
			dynamic_cast<EntityProjectile*>(CreateEntity(ENTITY_TYPE::PROJECTILE, "Projectile_" + std::to_string(i), mesh, material));
		projectile->transform.SetScale(0.15f, 0.15f, 0.15f);
		projectile->transform.SetPosition(0, 0, 200.0f);
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

	isColliding ? collisionManager->StageCollider(entity->GetCollider()) : collisionManager->UnstageCollider(entity->GetCollider());
}

void EntityFactory::SetEntityRendering(Entity* entity, bool isRendering)
{
	if (entity->isRendering == isRendering) {
		return;
	}
	entity->isRendering = isRendering;

	isRendering ? renderer->StageEntity(entity) : renderer->UnstageEntity(entity);
}

void EntityFactory::SetEntityUpdating(Entity* entity, bool isUpdating)
{
	if (entity->isUpdating == isUpdating) {
		return;
	}
	entity->isUpdating = isUpdating;

	isUpdating ? AddEntityToUpdate(entity) :RemoveEntityFromUpdate(entity);
}

void EntityFactory::SetCollisionManager(CollisionManager* collisionManager)
{
	this->collisionManager = collisionManager;
}

void EntityFactory::SetRenderer(Renderer* renderer)
{
	this->renderer = renderer;
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
	for (auto it = entities.begin(); it != entities.end(); it++)
		delete it->second;
}
