#pragma once
#include "Entity.h"
class EntityTest :
	public Entity
{
public:
	EntityTest(Mesh* mesh, Material* material);
	~EntityTest();

	void Update(float deltaTime, float totalTime);

	virtual void OnCollision(Collision collision);
};

