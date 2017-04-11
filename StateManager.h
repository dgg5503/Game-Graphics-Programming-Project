#pragma once
#include "Scene.h"
#include "GameState.h"
#include <unordered_map>

class StateManager
{
public:
	StateManager();
	~StateManager();

	GameState& GetCurrentState();
	
	void AddScene(GameState state, Scene* scene);
	void SetState(GameState newState, EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials);
private:
	GameState currentState;
	Scene* currentScene;



	std::unordered_map<GameState, Scene*> scenesMap;

	void LoadScene(Scene* scene, EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials);
	void UnloadScene(Scene* scene, EntityFactory& entityFactory);
};

