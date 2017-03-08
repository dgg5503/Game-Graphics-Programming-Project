#include "UIGamePanel.h"

UIGamePanel::UIGamePanel()
{
}

UIGamePanel::~UIGamePanel()
{
}

void UIGamePanel::Draw(SpriteBatch * const spriteBatch,
	const std::unordered_map<const char*, SpriteFont*>& fontMap)
{
	fontMap.at("arial")->DrawString(spriteBatch, L"Hello World :D!11!@#$%^&*()", XMFLOAT2(0, 0));
}

void UIGamePanel::Update(float deltaTime, float totalTime)
{
}
