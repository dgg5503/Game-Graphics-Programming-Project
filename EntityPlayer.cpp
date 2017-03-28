#include "EntityPlayer.h"



EntityPlayer::EntityPlayer(Mesh * mesh, Material * material) : Entity(mesh, material)
{
}

EntityPlayer::~EntityPlayer()
{
}

void EntityPlayer::Update(float deltaTime, float totalTime)
{
	// Move the player around
	if (GetAsyncKeyState('W') & 0x8000)
		transform.MoveForward(0, speed * deltaTime, 0);
	if (GetAsyncKeyState('S') & 0x8000)
		transform.MoveForward(0, -speed * deltaTime, 0);
	if (GetAsyncKeyState('D') & 0x8000)
		transform.MoveForward(speed * deltaTime, 0.0f, 0.0f);
	if (GetAsyncKeyState('A') & 0x8000)
		transform.MoveForward(-speed * deltaTime, 0.0f, 0.0f);
}

void EntityPlayer::SetSpeed(float speed)
{
	this->speed = speed;
}

float EntityPlayer::GetSpeed()
{
	return speed;
}

void EntityPlayer::OnCollision(Collision collision)
{
	printf("player");
}
