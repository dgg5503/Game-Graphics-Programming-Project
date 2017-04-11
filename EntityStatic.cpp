#include "EntityStatic.h"



EntityStatic::EntityStatic(Mesh * mesh, Material * material) : Entity(mesh, material)
{
	isUpdating = false;
}


EntityStatic::~EntityStatic()
{
}

void EntityStatic::Update(float deltaTime, float totalTime)
{
}
