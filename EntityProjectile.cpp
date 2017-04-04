#include "EntityProjectile.h"


EntityProjectile::EntityProjectile(Mesh * mesh, Material * material) : Entity(mesh, material)
{
	direction = XMFLOAT3(0, 0, 0);
	speed = 5.0f;
	this->AddTag("Projectile");
}

EntityProjectile::~EntityProjectile()
{
}


void EntityProjectile::Update(float deltaTime, float totalTime)
{
	XMFLOAT3 movement = XMFLOAT3();
	XMStoreFloat3(&movement, XMLoadFloat3(&direction) * speed * deltaTime);


	transform.Move(movement.x, movement.y, movement.z);
}

void EntityProjectile::Remove()
{
	transform.SetPosition(0, 0, -200);
}

void EntityProjectile::SetSpeed(float speed)
{
	this->speed = speed;
}

float EntityProjectile::GetSpeed()
{
	return this->speed;
}

void EntityProjectile::SetDirection(XMFLOAT3 direction)
{
	this->direction = direction;
}

XMFLOAT3 * EntityProjectile::GetDirection()
{
	return &this->direction;
}

void EntityProjectile::OnCollision(Collision collison)
{
	if (collison.otherEntity->HasTag("Enemy")) {
		Remove();
	}
}
