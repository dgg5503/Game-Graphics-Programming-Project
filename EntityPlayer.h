#pragma once
#include "Entity.h"
#include "ProjectileManager.h"

// Entity controlled by the player.
class EntityPlayer :
	public Entity
{
public:
	EntityPlayer(Mesh* mesh, Material* material);
	~EntityPlayer();

	// Set new update
	void Update(float deltaTime, float totalTime) override;
	
	// Setters and Getters
	void SetSpeed(float speed);
	float GetSpeed();
	void SetProjectileManager(ProjectileManager* projectileManager);

protected:
	float speed;		// The speed the player can move at
	float fireTimer;	// Stores time since last firing
	float fireRate;		// The maximun rate of firing
	ProjectileManager* projectileManager;	// Pointer to manager

	void FireProjectile(XMFLOAT3 direction);	// Fire a projectile

	void OnCollision(Collision collision) override;	
};

