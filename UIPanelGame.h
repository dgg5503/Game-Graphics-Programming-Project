#pragma once
#include <unordered_map>
#include "UIPanel.h"
#include "StateManager.h"
#define MAX_TIMER_BUFFER_LEN 9 // 00:00.00\0

class UIPanelGame : public UIPanel
{
public:
	UIPanelGame(float x, float y);
	virtual ~UIPanelGame();

	// Implement required functions
	void Draw(SpriteBatch* const spriteBatch,
		const std::unordered_map<const char*, SpriteFont*>& fontMap);
	void Update(float deltaTime, float totalTime);
	//void UpdateText(wstring _text);
	void UpdateHealth(int health);


	void SetGameTime(float gameTime);
private:
	float gameTime = 0;
	int health = 0;
	float x;
	float y;

	// Represents the healthbar
	struct HealthBar {
		SimpleMath::Rectangle rectangle;	// Represent position and scale.
		float maxWidth = 0;
		Texture2D* texture = nullptr;	// Texture of Button
	};
	HealthBar healthBar;
};