#include "EntityMeteor.h"
#include "Game.h"


EntityMeteor::EntityMeteor(EntityFactory * entityFactory, std::string name, Mesh * mesh, Material * material) :
	Entity(entityFactory, name, mesh, material)
{
	// start sound
	AK::SoundEngine::PostEvent(AK::EVENTS::MOVE_ASTEROID, id);
}

EntityMeteor::~EntityMeteor()
{
}

void EntityMeteor::Update(float deltaTime, float totalTime)
{
	// Center of 0,0,0
	// Convert position to spherical coords
	// Set rotation to look at 0,0,0
	// Spherical coords
	float x, y, z;
	float sinTheta, cosTheta;
	float theta;
	theta = totalTime / 5.0f;
	const float r = 25.0f;
	XMScalarSinCos(&sinTheta, &cosTheta, theta);
	x = r * sinTheta;
	y = 0;
	z = r * cosTheta;
	transform.SetPosition(x, y, z);
	transform.SetRotation(0, 1, 0, theta);

	// ensure in front of viewer
	const XMFLOAT2& screenCoords = Game::GetScreenCoords(transform);
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::POS_X, static_cast<AkRtpcValue>(screenCoords.x), id);
	AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::POS_Y, static_cast<AkRtpcValue>(screenCoords.y), id);
}
