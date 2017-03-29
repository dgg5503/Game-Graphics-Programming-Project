#include "EntityPlayer.h"



EntityPlayer::EntityPlayer(Mesh * mesh, Material * material) : Entity(mesh, material)
{
	fireRate = 0.1f;
	fireTimer = 0;
}

EntityPlayer::~EntityPlayer()
{
}

void EntityPlayer::Update(float deltaTime, float totalTime)
{
	// Move the player around
	XMFLOAT3 movement = XMFLOAT3(0,0,0);
	if (GetAsyncKeyState('W') & 0x8000)
		movement.y += 1.0;
	if (GetAsyncKeyState('S') & 0x8000)
		movement.y -= 1.0;
	if (GetAsyncKeyState('D') & 0x8000)
		movement.x += 1.0;
	if (GetAsyncKeyState('A') & 0x8000)
		movement.x -= 1.0;

	XMStoreFloat3(&movement, XMVector3Normalize(XMLoadFloat3(&movement)) * speed * deltaTime);

	transform.Move(movement.x, movement.y, movement.z);

	// Handle firing
	fireTimer += deltaTime;
	XMFLOAT3 fireDirection = XMFLOAT3(0, 0, 0);
	bool isFiring = false;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		fireDirection.y += 1.0f;
		isFiring = true;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		fireDirection.y -= 1.0f;
		isFiring = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		fireDirection.x += 1.0f;
		isFiring = true;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		fireDirection.x -= 1.0f;
		isFiring = true;
	}

	if (isFiring && fireTimer > fireRate) {
		fireProjectile(fireDirection);
		fireTimer = 0;
	}
}

void EntityPlayer::SetSpeed(float speed)
{
	this->speed = speed;
}

void EntityPlayer::SetProjectileManager(ProjectileManager* projectileManager)
{
	this->projectileManager = projectileManager;
}

float EntityPlayer::GetSpeed()
{
	return speed;
}

void EntityPlayer::OnCollision(Collision other)
{
}

void EntityPlayer::fireProjectile(XMFLOAT3 direction)
{
	EntityProjectile* projectile = projectileManager->GetProjectile();
	const XMFLOAT3* position = transform.GetPosition();

	projectile->transform.SetPosition(*position);
	projectile->SetDirection(direction);
	projectile->SetSpeed(5.0f);
}
