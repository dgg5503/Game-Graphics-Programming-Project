#pragma once
#include "Scene.h"
#include "StateManager.h"
#include "UIPanelMenu.h"

class StateManager;

class SceneMenu :
	public Scene
{
public:
	SceneMenu(StateManager& stateManager);
	virtual ~SceneMenu();
	void CreateSceneEntities(EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials) override;

	void OnMousePressed(float x, float y) override;
private:
	UIPanelMenu* uiPanel;
};

