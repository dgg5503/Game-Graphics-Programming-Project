#pragma once
#include "Entity.h"
class EntityMeteor :
	public Entity
{
public:
	EntityMeteor(EntityFactory* entityFactory, std::string name, Mesh * mesh, Material * material);
	~EntityMeteor();

	void Update(float deltaTime, float totalTime);
};

