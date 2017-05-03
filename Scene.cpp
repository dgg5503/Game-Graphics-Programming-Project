#include "Scene.h"
#include "MemoryDebug.h"

Scene::~Scene()
{
	if (uiPanel)
		delete uiPanel;
}

void Scene::UpdateScene(float deltaTime, float totalTime)
{
}

UIPanel* Scene::GetUIPanel()
{
	return this->uiPanel;
}
