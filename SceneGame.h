#pragma once
#include "Scene.h"
#include "UIPanelGame.h"

class SceneGame :
	public Scene
{
public:
	SceneGame(Game* game);
	virtual ~SceneGame();
	void CreateSceneEntities(EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials) override;
	void UpdateScene(float deltaTime, float totalTime) override;

	void OnMousePressed(float x, float y) override;

private:
	// Player
	EntityPlayer* player;

	//meteors
	Entity* meteors2;
	Entity* meteors;

	// User Interface
	UIPanelGame* uiGamePanel;

	//timer for ui
	wstring timerString;// [MAX_TIMER_BUFFER_LEN];
	float minutes = 0;
	float seconds = 0;
	float milliseconds = 0;

	int health = 100;
};

