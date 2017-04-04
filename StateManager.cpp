#include "StateManager.h"



StateManager::StateManager(GameState state)
{
	currentScene = scenesMap[state];
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



void StateManager::SetState(GameState newState)
{
	UnloadScene(currentScene);
	LoadScene(scenesMap[newState]);
	currentScene = scenesMap[newState];
}

void StateManager::LoadScene(Scene * scene)
{
	for (size_t i = 0; i < scene->GetEntityList().size(); i++)
	{
		Renderer::Instance()->StageEntity(scene->GetEntityList()[i]);
	}
	//set ui renderer junkety stuff 
}

void StateManager::UnloadScene(Scene * scene)
{
	for (size_t i = 0; i < scene->GetEntityList().size(); i++)
	{
		Renderer::Instance()->UnstageEntity(scene->GetEntityList()[i]);
	}
	//un set ui renderer junkety stuff 
}
