#include "EntityEnemy.h"


EntityEnemy::EntityEnemy(Mesh * mesh, Material * material) : Entity(mesh, material)
{
	this->health = 1.0f;
	this->healthMax = 1.0f;
}

EntityEnemy::~EntityEnemy()
{
}

void EntityEnemy::Update(float deltaTime, float totalTime)
{
	if (target != NULL) {
		XMStoreFloat3(&direction,
			XMVector3Normalize(XMLoadFloat3( target->transform.GetPosition()) - XMLoadFloat3(transform.GetPosition()))
			);


		transform.Move(direction.x * deltaTime, direction.y * deltaTime, direction.z * deltaTime);
	}

	// Set new scale
	float scale = (health ) / healthMax;
	transform.SetScale(DirectX::XMFLOAT3(scale, scale, scale));
}

void EntityEnemy::SetSpeed(float speed)
{
	this->speed = speed;
}

float EntityEnemy::GetSpeed()
{
	return this->speed;
}

void EntityEnemy::SetHealth(float health)
{
	this->health = health;
}

float EntityEnemy::GetHealth()
{
	return this->health;
}

void EntityEnemy::SetMaxHealth(float healthMax)
{
	this->healthMax = healthMax;
}

float EntityEnemy::GetMaxHealth()
{
	return this->healthMax;
}

void EntityEnemy::SetTarget(Entity* target)
{
	this->target = target;
}

Entity* EntityEnemy::GetTarget()
{
	return this->target;
}

void EntityEnemy::SetDirection(XMFLOAT3 direction)
{
	this->direction = direction;
}

const XMFLOAT3* const EntityEnemy::GetDirection() const
{
	return &this->direction;
}
