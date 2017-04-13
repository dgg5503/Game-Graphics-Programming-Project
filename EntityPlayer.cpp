#include "EntityPlayer.h"



EntityPlayer::EntityPlayer(Mesh* mesh, Material* material) : Entity(mesh, material)
{
	fireRate = 0.2f;
	fireTimer = 0;

	maxHealth = health = 100;
}

EntityPlayer::~EntityPlayer()
{
}

void EntityPlayer::Update(float deltaTime, float totalTime)
{
	// Move the player around
	XMFLOAT3 movement = XMFLOAT3(0,0,0);
	bool isSteering = false;
	if (GetAsyncKeyState('W') & 0x8000) {
		isSteering = true;
		movement.y += 1.0;
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		isSteering = true;
		movement.y -= 1.0;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		isSteering = true;
		movement.x += 1.0;
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		isSteering = true;
		movement.x -= 1.0;
	}
	XMStoreFloat3(&movement, XMVector3Normalize(XMLoadFloat3(&movement)) * speed * deltaTime);

	transform.Move(movement.x, movement.y, movement.z);
	transform.SetRotation(0, 0, 1, atan2f(movement.y, movement.x));

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
		FireProjectile(fireDirection);
		fireTimer = 0;
	}
}

void EntityPlayer::SetSpeed(float speed)
{
	this->speed = speed;
}

void EntityPlayer::SetProjectileManager(EntityManagerProjectile* projectileManager)
{
	this->projectileManager = projectileManager;
}

void EntityPlayer::ChangeHealth(int healthDelta)
{
	health += healthDelta;
	if (health <= 0) {
		health = 0;
		SetIsRendering(false);
		SetIsColliding(false);
	}
	else if (health > maxHealth) {
		health = maxHealth;
	}
}

float EntityPlayer::GetSpeed()
{
	return speed;
}

void EntityPlayer::OnCollision(Collision other)
{
	if (other.otherEntity->HasTag("Enemy")) {
		EntityEnemy* enemy = (EntityEnemy*)other.otherEntity;
		enemy->MoveToRandomPosition();	

		ChangeHealth(enemy->GetHealth() * -10);
		enemy->ChangeHealth(-1000);
	}
}

void EntityPlayer::FireProjectile(XMFLOAT3 direction)
{
	// Get projectile
	EntityProjectile* projectile = projectileManager->GetProjectile();
	const XMFLOAT3* position = transform.GetPosition();

	// Fire projectile
	projectile->Fire(*position, direction, 5.0f);
}
