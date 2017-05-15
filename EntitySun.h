#pragma once
#include "Entity.h"
#include "ParticleEmitter.h"

class EntitySun :
	public Entity
{
public:
	EntitySun(EntityFactory* entityFactory, std::string name, Mesh* mesh, Material* material);
	~EntitySun();
	
private:
	ParticleEmitter* emitter;
};

