#include "SceneGame.h"


SceneGame::SceneGame()
{
	uiPanel = gameUI = new UIGamePanel(0, 0);
}


void SceneGame::CreateSceneEntities(EntityFactory& entityFactory, std::unordered_map<const char*, Mesh*>& meshes, std::unordered_map<const char*, Material*>& materials)
{
	health = 100;

	// Projectile Entities
	EntityManagerProjectile* projectileManager =
		(EntityManagerProjectile*)entityFactory.CreateEntity(EntityType::MANAGER_PROJECTILE, "Projectile Manager");
	auto projectiles = entityFactory.CreateProjectileEntities(20, meshes["sphere"], materials["brick"]);
	projectileManager->SetProjectiles(projectiles);


	// Player entity
	player = (EntityPlayer*)entityFactory
		.CreateEntity(EntityType::PLAYER, "player", meshes["player"], materials["stone"]);
	player->SetSpeed(2.0f);
	player->SetProjectileManager(projectileManager);
	player->transform.SetPosition(0, 0, 0.0f);
	player->transform.SetScale(0.25f, 0.25f, 0.25f);
	player->SetCollider(Collider::ColliderType::SPHERE, XMFLOAT3(0.125f, 0.125f, 0.125f));

	EntityEnemy* enemy;
	for (auto i = 0u; i < 10; ++i) {
		enemy = (EntityEnemy*)entityFactory
			.CreateEntity(EntityType::ENEMY, "Enemy_" + std::to_string(i), meshes["enemy"], materials["sand"]);
		enemy->SetTarget(player);
		enemy->MoveToRandomPosition();
		enemy->transform.SetScale(0.25f, 0.25f, 0.25f);
		enemy->SetCollider(Collider::ColliderType::OBB);
	}

	// Background entity
	Entity* background = entityFactory
		.CreateEntity(EntityType::STATIC, "Background", meshes["cube"], materials["brick"]);
	background->transform.SetPosition(0, 0, 5.0f);
	background->transform.SetScale(8.0f, 5.0f, 1.0f);
}

void SceneGame::UpdateScene(float deltaTime, float totalTime)
{
	gameUI->UpdateHealth(player->health);

	if (player->health) {
		timerString = std::to_wstring(minutes) + L": " + std::to_wstring(seconds) + L": " + std::to_wstring(milliseconds);
		milliseconds += deltaTime * 1000;
		if (milliseconds >= 1000) {
			seconds++;
			milliseconds = 0;
		}
		if (seconds >= 60) {
			minutes++;
			seconds = 0;
		}
		gameUI->UpdateText(timerString);
	}
}
