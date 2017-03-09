#pragma once

#include <vector>
#include "Entity.h"
#include "Renderer.h"

class Scene
{
public:
	Scene();
	~Scene();
	
	// Load scene, stage all entities in entity list
	

private:
	// Vector of entities that this scene will load in when activated
	std::vector<Entity*> entityList;
};

