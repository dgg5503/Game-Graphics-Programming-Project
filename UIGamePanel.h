#pragma once
#include <unordered_map>
#include "UIPanel.h"

class UIGamePanel : public UIPanel
{
public:
	UIGamePanel();
	~UIGamePanel();

	// Implement required functions
	void Draw(SpriteBatch* const spriteBatch,
		const std::unordered_map<const char*, SpriteFont*>& fontMap);
	void Update(float deltaTime, float totalTime);
};