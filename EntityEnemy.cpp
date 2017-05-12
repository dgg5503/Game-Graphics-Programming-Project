#include "EntityEnemy.h"
#include "MemoryDebug.h"

using namespace DirectX;

EntityEnemy::EntityEnemy(EntityFactory* entityFactory, std::string name, Mesh * mesh, Material * material) : 
	Entity(entityFactory, name, mesh, material)
{
	this->AddTag("Enemy");

	this->speed = 1.0f;
	this->health = 0.000001f;
	this->healthMax = 1.0f;
	this->spawnTimer = 0.0f;


	// Explosion - occurs when killed
	Renderer* renderer = Renderer::Instance();
	peExplosionDebris = renderer->CreateBurstParticleEmitter("PE_" + name + "_Explosion_Debris", 20);
	peExplosionDebris->SetAgeRange(2.0f, 1.0f);
	peExplosionDebris->SetAlpha(1.0f);
	peExplosionDebris->SetInitialSpeedRange(0.05f, 0.15f);
	peExplosionDebris->SetInitialTintRange(XMFLOAT3(0.25, 0.25f, 0.25f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	peExplosionDebris->SetInterpSize(true);
	peExplosionDebris->SetInitialSizeRange(XMFLOAT2(0.5f, 0.5f), XMFLOAT2(0.75f, 0.75f));
	peExplosionDebris->SetEndSize(XMFLOAT2(0, 0));
	peExplosionDebris->SetDirectionRange(XMFLOAT3(1, 1, 1), XMFLOAT3(-1, -1, -1));

	peExplosionFireball = renderer->CreateBurstParticleEmitter("PE_" + name + "_Explosion_Fireball", 20);
	peExplosionFireball->SetAgeRange(2.0f, 1.0f);
	peExplosionFireball->SetAlpha(0.75f);
	peExplosionFireball->SetInitialSpeedRange(0.05f, 0.1f);
	peExplosionFireball->SetInitialTintRange(XMFLOAT3(0.93f, 0.09f, 0.09f), XMFLOAT3(0.95f, .55f, .05f));
	peExplosionFireball->SetInterpSize(true);
	peExplosionFireball->SetInitialSizeRange(XMFLOAT2(0.5f, 0.5f), XMFLOAT2(0.75f, 0.75f));
	peExplosionFireball->SetEndSize(XMFLOAT2(0, 0));
	peExplosionFireball->SetDirectionRange(XMFLOAT3(1, 1, 1), XMFLOAT3(-1, -1, -1));

	// Set spawn timer to 0
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::ENEMY_SPAWN_TIMER, static_cast<AkRtpcValue>(this->spawnTimer));

	// Fire audio
	AK::SoundEngine::PostEvent(AK::EVENTS::MOVE_ENEMY, id);

}

EntityEnemy::~EntityEnemy()
{
	// Stop looping audio
	// Fire audio
	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MOVE_ENEMY, id);
}

void EntityEnemy::Update(float deltaTime, float totalTime)
{
	// Increment spawn timer
	spawnTimer += 0.25f * deltaTime;
	spawnTimer = AkClamp(spawnTimer / healthMax, 0.0f, 1.0f);
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::ENEMY_SPAWN_TIMER, static_cast<AkRtpcValue>(spawnTimer));

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

	// Update position in sound engine
	AK::SoundEngine::SetPosition(id, transform);
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
		spawnTimer = 0;

		// Explosion Effect
		const XMFLOAT3* position(transform.GetPosition());

		peExplosionDebris->SetPosition(*position);
		peExplosionFireball->SetPosition(*position);
		peExplosionDebris->Emit();
		peExplosionFireball->Emit();

		// Fire audio
		AK::SoundEngine::PostEvent(AK::EVENTS::EXPLODE_ENEMY, id);

		// Spawn in new location
		MoveToRandomPosition();
	}
	else if (health > healthMax) {
		health = healthMax;
	}

	// Set new scale
	float scale = (health) / healthMax * .5f;
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::ENEMY_SCALE, static_cast<AkRtpcValue>(health / healthMax), id);
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

		// Fire audio
		AK::SoundEngine::PostEvent(AK::EVENTS::LASER_HIT_ENEMY, id);
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
