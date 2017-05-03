#include "EntityStatic.h"
#include "MemoryDebug.h"

EntityStatic::EntityStatic(EntityFactory* entityFactory, std::string name, Mesh * mesh, Material * material) : 
	Entity(entityFactory, name, mesh, material)
{
	SetIsUpdating(false);
}


EntityStatic::~EntityStatic()
{
}

void EntityStatic::Update(float deltaTime, float totalTime)
{
}
