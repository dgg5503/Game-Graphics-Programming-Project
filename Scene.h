#pragma once

#include <vector>
#include "EntityFactory.h"
#include "Renderer.h"
#include "UIPanel.h"

class Scene
{
public:
	virtual void CreateSceneEntities(EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials) = 0;

private:
	// Vector of entities that this scene will load in when activated
	std::vector<Entity*> entityList;
	UIPanel* uiPanel;
};

