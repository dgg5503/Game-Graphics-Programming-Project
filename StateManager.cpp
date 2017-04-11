#include "StateManager.h"



StateManager::StateManager()
{
}

StateManager::~StateManager()
{
}

GameState& StateManager::GetCurrentState()
{
	return currentState;
}

void StateManager::AddScene(GameState state, Scene * scene)
{
	if (scenesMap[state] == nullptr)
	{
		scenesMap[state] = scene;
	}
}



void StateManager::SetState(GameState newState, EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials)
{
	UnloadScene(currentScene, entityFactory);
	LoadScene(scenesMap[newState], entityFactory, meshes, materials);
	currentScene = scenesMap[newState];
}

void StateManager::LoadScene(Scene* scene, EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials)
{
	scene->CreateSceneEntities(entityFactory, meshes, materials);
	//set ui renderer junkety stuff 
}

void StateManager::UnloadScene(Scene* scene, EntityFactory& entityFactory)
{
	entityFactory.Release();
}
