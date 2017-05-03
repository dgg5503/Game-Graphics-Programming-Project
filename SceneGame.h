#pragma once
#include "Scene.h"

class SceneGame :
	public Scene
{
public:
	SceneGame();
	virtual ~SceneGame();
	void CreateSceneEntities(EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials) override;
	void UpdateScene(float deltaTime, float totalTime) override;

private:
	// Player
	EntityPlayer* player;

	//meteors
	Entity* meteors2;
	Entity* meteors;

	// User Interface
	UIGamePanel* uiGamePanel;

	//timer for ui
	wstring timerString;// [MAX_TIMER_BUFFER_LEN];
	float minutes = 0;
	float seconds = 0;
	float milliseconds = 0;

	int health = 100;
};

