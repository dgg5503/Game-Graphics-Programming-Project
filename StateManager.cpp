#include "StateManager.h"



StateManager::StateManager()
{
}

StateManager::~StateManager()
{
	for (auto iter = scenesMap.begin(); iter != scenesMap.end(); ++iter)
	{
		delete iter->second;
	}
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

Scene* StateManager::GetCurrentScene()
{
	return currentScene;
}


void StateManager::SetState(GameState newState, EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials)
{
	UnloadScene(currentScene, entityFactory);
	currentScene = scenesMap[newState];
	LoadScene(currentScene, entityFactory, meshes, materials);
}

void StateManager::LoadScene(Scene* scene, EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials)
{
	scene->CreateSceneEntities(entityFactory, meshes, materials);
	//set ui renderer junkety stuff 
	Renderer::Instance()->SetCurrentPanel(currentScene->GetUIPanel());
}

void StateManager::UnloadScene(Scene* scene, EntityFactory& entityFactory)
{
	entityFactory.Release();
	Renderer::Instance()->ReleaseParticleRenderer();
}
