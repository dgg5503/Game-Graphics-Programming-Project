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
	// Parse time as string
	//snprintf(timerString, MAX_TIMER_BUFFER_LEN, "%00i:%00i.%00f", minutes, seconds, miliseconds;

	// Measure string for centering to screen
	//fontMap.at("arial")->MeasureString(timerString) // Measure 
	
	// Print time and other game related UI stuff
	fontMap.at("arial")->DrawString(spriteBatch, L"Hello World :D!11!@#$%^&*()", XMFLOAT2(0, 0));
}

void UIGamePanel::Update(float deltaTime, float totalTime)
{
}
