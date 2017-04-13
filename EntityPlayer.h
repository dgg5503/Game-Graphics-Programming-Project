#pragma once
#include "Entity.h"
#include "EntityManagerProjectile.h"
#include "EntityEnemy.h"

// Entity controlled by the player.
class EntityPlayer :
	public Entity
{
public:
	EntityPlayer(EntityFactory* entityFactory, std::string name, Mesh* mesh, Material* material);
	~EntityPlayer();

	// Set new update
	void Update(float deltaTime, float totalTime) override;
	
	// Setters and Getters
	void SetSpeed(float speed);
	float GetSpeed();
	void SetProjectileManager(EntityManagerProjectile* projectileManager);

	// Change health by amount
	void ChangeHealth(int healthDelta);

	int health, maxHealth;

protected:
	float speed;		// The speed the player can move at
	float fireTimer;	// Stores time since last firing
	float fireRate;		// The maximum rate of firing
	EntityManagerProjectile* projectileManager;	// Pointer to manager

	void FireProjectile(XMFLOAT3 direction);	// Fire a projectile

	void OnCollision(Collision collision) override;	
};

