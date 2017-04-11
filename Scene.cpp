#include "Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}

std::vector<Entity*> Scene::GetEntityList()
{
	return entityList;
}

UIPanel * Scene::GetUIPanel()
{
	return uiPanel;
}
