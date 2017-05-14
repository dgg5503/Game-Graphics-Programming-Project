#include "EntityPlayer.h"
#include "MemoryDebug.h"

EntityPlayer::EntityPlayer(EntityFactory* entityFactory, std::string name, Mesh* mesh, Material* material) :
	Entity(entityFactory, name, mesh, material)
{
	fireRate = 0.15f;
	fireTimer = 0;
	maxHealth = health = 100;

#pragma region Particle Effects
	Renderer* renderer = Renderer::Instance();
	// Engine exhaust - Used when the player is moving
	peEngineExhaust = renderer->CreateContinuousParticleEmitter("PE_Player_Movement", 1, 0.01f);
	peEngineExhaust->SetAgeRange(.25f, 0.01f);
	peEngineExhaust->SetAlpha(1.0f);
	peEngineExhaust->SetInitialSpeedRange(1.0f, 2.0f);
	peEngineExhaust->SetInterpTint(true);
	peEngineExhaust->SetInitialTintRange(XMFLOAT3(0.95f, .55f, .05f), XMFLOAT3(1.0f, .65f, .15f));
	peEngineExhaust->SetEndTintRange(XMFLOAT3(0.5f, .5f, .5f), XMFLOAT3(0.25f, 0.25f, 0.25f));
	peEngineExhaust->SetInterpSize(true);
	peEngineExhaust->SetInitialSizeRange(XMFLOAT2(.075f, .075f), XMFLOAT2(.2f, .2f));
	peEngineExhaust->SetEndSize(XMFLOAT2(0, 0));

	// Fire Projectile - Burst when a projectile is fired
	peFireProjectile = renderer->CreateBurstParticleEmitter("PE_Fire Projectile", 200);
	peFireProjectile->SetAgeRange(.10f, 0.25f);
	peFireProjectile->SetAlpha(0.75f);
	peFireProjectile->SetInitialSpeedRange(1.0f, 2.0f);
	peFireProjectile->SetInitialTintRange(XMFLOAT3(0.93f, 0.09f, 0.09f), XMFLOAT3(1.0f, .15f, .15f));
	peFireProjectile->SetInterpSize(true);
	peFireProjectile->SetInitialSizeRange(XMFLOAT2(.075f, .075f), XMFLOAT2(.2f, .2f));
	peFireProjectile->SetEndSize(XMFLOAT2(0, 0));

	// Explosion - occurs when killed
	peExplosionDebris = renderer->CreateBurstParticleEmitter("PE_Explosion_Debris", 200);
	peExplosionDebris->SetAgeRange(2.0f, 1.0f);
	peExplosionDebris->SetAlpha(1.0f);
	peExplosionDebris->SetInitialSpeedRange(5.0f, 10.0f);
	peExplosionDebris->SetInitialTintRange(XMFLOAT3(0.25, 0.25f, 0.25f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	peExplosionDebris->SetInterpSize(true);
	peExplosionDebris->SetInitialSizeRange(XMFLOAT2(0.5f, 0.5f), XMFLOAT2(0.75f, 0.75f));
	peExplosionDebris->SetEndSize(XMFLOAT2(0, 0));
	peExplosionDebris->SetDirectionRange(XMFLOAT3(1, 1, 1), XMFLOAT3(-1, -1, -1));

	peExplosionFireball = renderer->CreateBurstParticleEmitter("PE_Explosion_Fireball", 200);
	peExplosionFireball->SetAgeRange(2.0f, 1.0f);
	peExplosionFireball->SetAlpha(0.75f);
	peExplosionFireball->SetInitialSpeedRange(1.0f, 2.0f);
	peExplosionFireball->SetInitialTintRange(XMFLOAT3(0.93f, 0.09f, 0.09f), XMFLOAT3(0.95f, .55f, .05f));
	peExplosionFireball->SetInterpSize(true);
	peExplosionFireball->SetInitialSizeRange(XMFLOAT2(0.5f, 0.5f), XMFLOAT2(0.75f, 0.75f));
	peExplosionFireball->SetEndSize(XMFLOAT2(0, 0));
	peExplosionFireball->SetDirectionRange(XMFLOAT3(1, 1, 1), XMFLOAT3(-1, -1, -1));
#pragma endregion 
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

	if (isSteering) {
		auto emitPosition = transform.GetPosition();
		XMFLOAT3 backwards = XMFLOAT3();
		XMStoreFloat3(&backwards, XMVector3Normalize(-XMLoadFloat3(&movement)));
		XMFLOAT3 backwardsLeft = backwards;
		XMFLOAT3 backwardsRight = backwards;
		backwardsLeft.x -= 0.25f;
		backwardsLeft.y -= 0.25f;
		backwardsRight.x += 0.25f;
		backwardsRight.y += 0.25f;

		peEngineExhaust->SetDirectionRange(backwardsLeft, backwardsRight);
		peEngineExhaust->SetPosition(*emitPosition);
		peEngineExhaust->SetLoop(-1);
		peEngineExhaust->Emit();
	}
	else {
		peEngineExhaust->SetLoop(0);
		peEngineExhaust->Emit();
	}

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
		SetIsUpdating(false);

		// Explosion Effect
		const XMFLOAT3* position = transform.GetPosition();

		peExplosionDebris->SetPosition(*position);
		peExplosionFireball->SetPosition(*position);
		peExplosionDebris->Emit();
		peExplosionFireball->Emit();
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

		// Sufficiently weak enemies inflict no damage.
		ChangeHealth(static_cast<int>(enemy->GetHealth()) * -10);
		enemy->ChangeHealth(-1000);	// Kill enemy
	}
}

void EntityPlayer::FireProjectile(XMFLOAT3 direction)
{
	// Get projectile
	EntityProjectile* projectile = projectileManager->GetProjectile();
	const XMFLOAT3* position = transform.GetPosition();

	// Fire projectile
	projectile->Fire(*position, direction, 5.0f);

	// Burst particle effect
	XMFLOAT3 directionLeft = direction;
	XMFLOAT3 directionRight = direction;
	directionLeft.x -= 0.75f;
	directionLeft.y -= 0.75f;
	directionRight.x += 0.75f;
	directionRight.y += 0.75f;

	peFireProjectile->SetDirectionRange(directionLeft, directionRight);
	peFireProjectile->SetPosition(*position);
	peFireProjectile->Emit();

	// Fire audio
	AK::SoundEngine::PostEvent(AK::EVENTS::FIRE_LASER_PLAYER, id);
}
