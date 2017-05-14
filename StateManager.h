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
	Scene* GetCurrentScene();

	// Setters
	void SetState(GameState newState);
	void SetEntityFactory(EntityFactory* entityFactory);
	void SetMeshes(std::unordered_map<const char*, Mesh*>* meshes);
	void SetMaterials(std::unordered_map<const char*, Material*>* materials);
private:
	EntityFactory* entityFactory; 
	std::unordered_map<const char*, Mesh*>* meshes;
	std::unordered_map<const char*, Material*>* materials;

	GameState currentState;
	Scene* currentScene = nullptr;

	std::unordered_map<GameState, Scene*> scenesMap;

	void LoadScene(Scene* scene);
	void UnloadScene(Scene* scene);
};

