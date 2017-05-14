#pragma once
#include <vector>
#include "EntityFactory.h"
#include "Renderer.h"
#include "UIPanel.h"

class Scene
{
public:
	virtual ~Scene();
	virtual void CreateSceneEntities(EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials) = 0;

	virtual void UpdateScene(float deltaTime, float totalTime);
	virtual void OnMousePressed(float x, float y);

	UIPanel* GetUIPanel();

protected:
	// Vector of entities that this scene will load in when activated
	std::vector<Entity*> entityList;
	UIPanel* uiPanel;
};

