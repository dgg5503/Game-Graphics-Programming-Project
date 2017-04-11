#pragma once
#include <unordered_map>

// Entities
#include "Entity.h"
#include "EntityPlayer.h"
#include "EntityEnemy.h"
#include "EntityProjectile.h"
#include "EntityStatic.h"

// Managers
#include "CollisionManager.h"
#include "Renderer.h"

class Mesh;
class Material;

enum ENTITY_TYPE {
	STATIC,
	ENEMY,
	PLAYER,
	PROJECTILE,
};


class EntityFactory
{
protected:
	 std::unordered_map<std::string, Entity*> entities;
	 
	 CollisionManager* collisionManager;

public:
	void Release();
	
	Entity* CreateEntity(ENTITY_TYPE entityType, std::string name, Mesh* mesh = nullptr, Material* material = nullptr);
	std::vector<EntityProjectile*> CreateProjectileEntities(unsigned int numberOfProjectiles, Mesh* mesh = nullptr, Material* material = nullptr);

	void UpdateEntities(float deltaTime, float  totalTime);

	void ChangeEntityCollision(Entity* entity, bool isColliding);

	void SetCollisionManager(CollisionManager* collisionManager);

	std::unordered_map<std::string, Entity*> GetEntities();

};

