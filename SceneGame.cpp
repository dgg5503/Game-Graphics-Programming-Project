#include "SceneGame.h"


SceneGame::SceneGame()
{
	uiPanel = uiGamePanel = new UIGamePanel(0, 0);
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

	//// Background entity
	//Entity* background = entityFactory
	//	.CreateEntity(EntityType::STATIC, "Background", meshes["cube"], materials["brick"]);
	//background->transform.SetPosition(0, 0, 5.0f);
	//background->transform.SetScale(8.0f, 5.0f, 1.0f);
	float s = 1.0; 

	//frame entity
	Entity* frame = entityFactory
		.CreateEntity(EntityType::STATIC, "Frame", meshes["frame"], materials["stone"]);
	frame->transform.SetPosition(0, 0, 0);
	frame->transform.SetScale(.22f, .3f, 1.0f);

	//planet entity
	Entity* planet = entityFactory
		.CreateEntity(EntityType::STATIC, "Planet", meshes["planet"], materials["stone"]);
	planet->transform.SetPosition(0, 0, 28.0f);
	planet->transform.SetScale(s, s, s);

	Entity* sun = entityFactory
		.CreateEntity(EntityType::STATIC, "Sun", meshes["sun"], materials["sand"]);
	sun->transform.SetPosition(0, 0, 28.0f);
	sun->transform.SetScale(s, s, s);

	//meteors
	meteors = entityFactory
		.CreateEntity(EntityType::STATIC, "meteors", meshes["meteors"], materials["stone"]);
	meteors->transform.SetPosition(0, 0, 0);
	meteors->transform.SetScale(s, s, s);

	//meteors2 to fill it out a bit
	meteors2 = entityFactory
		.CreateEntity(EntityType::STATIC, "meteors2", meshes["meteors"], materials["stone"]);
	meteors2->transform.SetPosition(0, 0, 0);
	meteors2->transform.SetScale(s, s, s);
}

void SceneGame::UpdateScene(float deltaTime, float totalTime)
{
	uiGamePanel->UpdateHealth(player->health);

	//spin meteors
	meteors->transform.SetRotation(0,1,0, totalTime/20);
	meteors2->transform.SetRotation(0, 1, 0, (totalTime / 20)+ 80);


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
		uiGamePanel->UpdateText(timerString);
	}
}
