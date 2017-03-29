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
	
	void SetSpeed(float speed);
	void SetProjectileManager(ProjectileManager* projectileManager);
	float GetSpeed();

protected:
	float speed;
	float fireTimer, fireRate;
	ProjectileManager* projectileManager;

	void OnCollision(Collision collision) override;
	void fireProjectile(XMFLOAT3 direction);
};

