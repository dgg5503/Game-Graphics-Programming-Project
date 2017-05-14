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

	struct Button {
		SimpleMath::Rectangle rectangle;
		wstring text;
		Texture2D* texture;
		void(UIPanelMenu::*onPressed) (void);

		bool CheckIsPressed(float x, float y) {
			return	rectangle.x <= x && 
				x <= rectangle.x + rectangle.width &&
				rectangle.y <= y &&
				y <= rectangle.y + rectangle.height;
		}
	};

	Button startGame;
	Button exitGame;

	void PlayGame();
	void ExitGame();
};

