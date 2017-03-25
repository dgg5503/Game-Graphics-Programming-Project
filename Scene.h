#pragma once

#include <vector>
#include "Entity.h"
#include "Renderer.h"
#include "UIPanel.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Entity*> GetEntityList();

private:
	// Vector of entities that this scene will load in when activated
	std::vector<Entity*> entityList;
	UIPanel* uiPanel;
};

