#include "SceneMenu.h"

void SceneMenu::CreateSceneEntities(EntityFactory & entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials)
{
	// Background entity
	for (auto i = 0u; i < 100; ++i) {
		Entity* background = entityFactory
			.CreateEntity(EntityType::STATIC, "Background_" + std::to_string(i), meshes["cube"], materials["brick"]);
		background->transform.SetPosition(rand() % 10000 / 1000.0f - 5, rand() % 10000 / 1000.0f - 5, 5.0f);
		background->transform.SetScale(0.5f, 0.5f, 0.5f);
	}
}
