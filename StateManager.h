#pragma once
#include "Scene.h"
#include "GameState.h"
#include <unordered_map>

class StateManager
{
public:
	StateManager(GameState state);
	~StateManager();

	GameState& GetCurrentState();
	
	void AddScene(GameState state, Scene* scene);
	void SetState(GameState newState);
private:
	GameState currentState;
	Scene* currentScene;

	std::unordered_map<GameState, Scene*> scenesMap;

	void LoadScene(Scene* scene);
	void UnloadScene(Scene* scene);

};

