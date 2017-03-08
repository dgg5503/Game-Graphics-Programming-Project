#pragma once
#include "Entity.h"

// Entity controlled by the player.
class EntityPlayer :
	public Entity
{
public:
	EntityPlayer(Mesh* mesh, Material* material);
	~EntityPlayer();

	// Set new update
	void Update(float deltaTime, float totalTime) override;
	
	void SetSpeed(float speed);
	float GetSpeed();

protected:
	float speed;
};

