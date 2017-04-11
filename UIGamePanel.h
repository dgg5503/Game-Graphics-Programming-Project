#pragma once
#include <unordered_map>
#include "UIPanel.h"
#define MAX_TIMER_BUFFER_LEN 9 // 00:00.00\0

class UIGamePanel : public UIPanel
{
public:
	UIGamePanel();
	virtual ~UIGamePanel();

	// Implement required functions
	void Draw(SpriteBatch* const spriteBatch,
		const std::unordered_map<const char*, SpriteFont*>& fontMap);
	void Update(float deltaTime, float totalTime);
	void UpdateText(wstring _text);

private:
	wstring text;
};