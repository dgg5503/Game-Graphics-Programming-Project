#pragma once
#include "Entity.h"
class TestEntity :
	public Entity
{
public:
	TestEntity(Mesh* mesh, Material* material);
	~TestEntity();

	void Update(float deltaTime, float totalTime);
};

