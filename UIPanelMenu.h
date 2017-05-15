#pragma once
#include "UIPanel.h"
#include "StateManager.h"


class UIPanelMenu :
	public UIPanel
{
public:
	UIPanelMenu(StateManager* stateManager);
	~UIPanelMenu();
	void Draw(SpriteBatch* const spriteBatch,
		const std::unordered_map<const char*, SpriteFont*>& fontMap) override;

	void MousePressed(float x, float y);

private:
	StateManager* stateManager;

	// Represents a UI element that can be pressed.
	struct Button {
		SimpleMath::Rectangle rectangle;	// Represent position and scale.
		wstring text;	// Text of Button
		Texture2D* texture = nullptr;	// Texture of Button
		void(UIPanelMenu::*onPressed) (void) = nullptr;	// Function called when pressed

		// Check if it is pressed.
		bool CheckIsPressed(float x, float y) {
			return	rectangle.x <= x && 
				x <= rectangle.x + rectangle.width &&
				rectangle.y <= y &&
				y <= rectangle.y + rectangle.height;
		}
	};

	std::unordered_map<const char*, Button> buttons;

	void PlayGame();
	void ExitGame();
};

