#pragma once
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <unordered_map>
#include <iostream>
#include <string>
using std::wstring;

using namespace DirectX;

// Basic interface that every UI element will need to implement
class UIPanel
{
public:
	// Required to be implemented UI Panel draw and update functions
	virtual ~UIPanel() {};
	virtual void Draw(SpriteBatch* const spriteBatch,
		const std::unordered_map<const char*, SpriteFont*>& fontMap) = 0;
	virtual void Update(float deltaTime, float totalTime) = 0;
	virtual void UpdateText(wstring text) = 0;
};

