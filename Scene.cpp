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

void Scene::OnMousePressed(float x, float y)
{
}

UIPanel* Scene::GetUIPanel()
{
	return this->uiPanel;
}
