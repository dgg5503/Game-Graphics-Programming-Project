#include "EntityProjectile.h"
#include "EntityFactory.h"
#include "MemoryDebug.h"

EntityProjectile::EntityProjectile(EntityFactory* entityFactory, std::string name, Mesh* mesh, Material* material) :
	Entity(entityFactory, name, mesh, material)
{
	direction = XMFLOAT3(0, 0, 0);
	speed = 5.0f;
	this->AddTag("Projectile");
	SetIsUpdating(true);

	// Particle Effect
	peTrail = Renderer::Instance()->CreateContinuousParticleEmitter("pe_" + name + "_trail", 5, 0.01f);
	peTrail->SetAgeRange(0.01f, 0.25f);
	peTrail->SetAlpha(1.0f);
	peTrail->SetInitialSpeedRange(1.0f, 2.0f);
	peTrail->SetInterpTint(true);
	peTrail->SetInitialTintRange(XMFLOAT3(0.95f, .55f, .05f), XMFLOAT3(1.0f, .65f, .15f));
	peTrail->SetEndTintRange(XMFLOAT3(0.5f, .5f, .5f), XMFLOAT3(0.25f, 0.25f, 0.25f));
	peTrail->SetInterpSize(true);
	peTrail->SetInitialSizeRange(XMFLOAT2(.075f, .075f), XMFLOAT2(.2f, .2f));
	peTrail->SetEndSize(XMFLOAT2(0, 0));
}

EntityProjectile::~EntityProjectile()
{
}


void EntityProjectile::Update(float deltaTime, float totalTime)
{
	XMFLOAT3 movement = XMFLOAT3();
	XMStoreFloat3(&movement, XMLoadFloat3(&direction) * speed * deltaTime);


	transform.Move(movement.x, movement.y, movement.z);

	// Particle Effect
	const XMFLOAT3* position = transform.GetPosition();
	XMFLOAT3 backwards = XMFLOAT3();
	XMStoreFloat3(&backwards, XMVector3Normalize(-XMLoadFloat3(&movement)));
	XMFLOAT3 backwardsLeft = backwards;
	XMFLOAT3 backwardsRight = backwards;
	backwardsLeft.x -= 0.25f;
	backwardsLeft.y -= 0.25f;
	backwardsRight.x += 0.25f;
	backwardsRight.y += 0.25f;

	peTrail->SetDirectionRange(backwardsLeft, backwardsRight);
	peTrail->SetPosition(*position);
}

void EntityProjectile::Fire(XMFLOAT3 position, XMFLOAT3 direction, float speed)
{
	transform.SetPosition(position);
	SetDirection(direction);
	SetSpeed(speed);

	SetIsUpdating(true);
	SetIsColliding(true);

	peTrail->SetPosition(position);
	peTrail->SetLoop(-1);
	peTrail->Emit();
}

void EntityProjectile::Remove()
{
	transform.SetPosition(0, 0, -200);
	SetIsUpdating(false);
	SetIsColliding(false);
	peTrail->SetLoop(0);
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
