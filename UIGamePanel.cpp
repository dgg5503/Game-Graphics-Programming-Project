#include "UIGamePanel.h"

UIGamePanel::UIGamePanel(float _x, float _y)
{
	x = _x;
	y = _y;
}

UIGamePanel::~UIGamePanel()
{
}

void UIGamePanel::Draw(SpriteBatch * const spriteBatch,
	const std::unordered_map<const char*, SpriteFont*>& fontMap)
{
	// Parse time as string
	//snprintf(timerString, MAX_TIMER_BUFFER_LEN, "%00i:%00i.%00f", minutes, seconds, milliseconds);
	// Measure string for centering to screen
	//fontMap.at("arial")->MeasureString(timerString) // Measure 
	
	// Print time and other game related UI stuff
	fontMap.at("arial")->DrawString(spriteBatch, text.c_str() , XMFLOAT2(x, y));
}

void UIGamePanel::Update(float deltaTime, float totalTime)
{
}

void UIGamePanel::UpdateText(wstring _text)
{
	text = _text;
}
