#pragma once
#include "Entity.h"
#include "MaterialEnemy.h"

class EntityEnemy :
	public Entity
{
public:
	EntityEnemy(EntityFactory* entityFactory, std::string name, Mesh* mesh, Material* material);
	~EntityEnemy();

	void Update(float deltaTime, float totalTime) override;

	// Move to random position.
	void MoveToRandomPosition();

	// Change health by amount
	void ChangeHealth(float healthDelta);

	// Setters and Getters
	void SetSpeed(float speed);
	float GetSpeed();
	void SetHealth(float health);
	float GetHealth();
	void SetMaxHealth(float healthMax);
	float GetMaxHealth();
	void SetTarget(Entity* target);
	Entity* GetTarget();
	void SetDirection(XMFLOAT3 direction);
	const XMFLOAT3* const GetDirection() const;

protected:
	float maxScale;		// Max scale of enemy
	float speed;		// Speed the enemy moves at
	float health;		// The enemies current health.
	float healthMax;	// The max health of the enemy
	float spawnTimer;	// When respawning, start the timer for pitch

	Entity* target;		// The target of the enemy
	XMFLOAT3 direction;	// Direction the enemy is looking at
	XMFLOAT3 rotationAxis;

	// Material
	MaterialEnemy* enemyMaterial;

	// Particle Systems
	ParticleEmitter* peExplosionDebris;
	ParticleEmitter* peExplosionFireball;

	void OnCollision(Collision collision) override;
};

