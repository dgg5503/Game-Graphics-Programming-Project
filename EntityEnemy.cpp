#include "EntityEnemy.h"


EntityEnemy::EntityEnemy(EntityFactory* entityFactory, std::string name, Mesh * mesh, Material * material) : 
	Entity(entityFactory, name, mesh, material)
{
	this->AddTag("Enemy");

	this->speed = 1.0f;
	this->health = 0;
	this->healthMax = 1.0f;
}

EntityEnemy::~EntityEnemy()
{
}

void EntityEnemy::Update(float deltaTime, float totalTime)
{
	// If there is a target
	if (target != NULL) {
		XMStoreFloat3(
			&direction,	// Store the direction towards the target
			XMVector3Normalize(XMLoadFloat3( target->transform.GetPosition()) - XMLoadFloat3(transform.GetPosition()))
			);

		// Move towards target
		transform.Move(direction.x * deltaTime * speed, direction.y * deltaTime * speed, direction.z * deltaTime * speed);
	}

	transform.SetRotation(1, 1, 1, totalTime);

	// Regenerate health overtime
	ChangeHealth(0.25f * deltaTime);
}

void EntityEnemy::MoveToRandomPosition()
{
	transform.SetPosition(rand() % 10 - 5.0f, rand() % 10 - 5.0f, 0.0f);
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

void EntityEnemy::ChangeHealth(float healthDelta)
{
	health += healthDelta;

	if (health <= 0) {
		health = 0;
		// Spawn in new location
		MoveToRandomPosition();
	}
	else if (health > healthMax) {
		health = healthMax;
	}

	// Set new scale
	float scale = (health) / healthMax * .5f;
	transform.SetScale(DirectX::XMFLOAT3(scale, scale, scale));
	GetCollider()->SetScale(DirectX::XMFLOAT3(scale/2, scale/2, scale/2));
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

void EntityEnemy::OnCollision(Collision collision) {
	// Colliding with projectile
	if (collision.otherEntity->HasTag("Projectile"))
	{
		// Take damage
		ChangeHealth(-.2f);
	}

	// Colliding with enemy
	else if (collision.otherEntity->HasTag("Enemy"))
	{
		// Bounce off enemy
		const XMFLOAT3* otherPosition = collision.otherTransform.GetPosition();
		XMFLOAT3 bounce = XMFLOAT3();
		XMVECTOR bounceVector = XMLoadFloat3(transform.GetPosition()) - XMLoadFloat3(otherPosition);
		bounceVector = XMVector3Normalize(bounceVector) * 0.005f;
		XMStoreFloat3(&bounce, bounceVector);

		// Move away from other enemy
		transform.Move(bounce.x, bounce.y, bounce.z);
	}
}
