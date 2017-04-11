#pragma once
#include "Scene.h"
class SceneMenu :
	public Scene
{
public:
	void CreateSceneEntities(EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials) override;
};

