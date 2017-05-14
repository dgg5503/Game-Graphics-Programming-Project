#include "UIPanelMenu.h"

UIPanelMenu::UIPanelMenu(StateManager* stateManager) :
	stateManager(stateManager)
{
	startGame.rectangle = SimpleMath::Rectangle(576, 300, 128, 64);
	startGame.texture = Renderer::Instance()->CreateTexture2D(L"./Assets/Textures/TexturesCom_BrownBricks_albedo_M.tif", Texture2DType::ALBEDO);
	startGame.text = L"Play Game";
	startGame.onPressed = &UIPanelMenu::PlayGame;


	exitGame.rectangle = SimpleMath::Rectangle(576, 400, 128, 64);
	exitGame.texture = Renderer::Instance()->CreateTexture2D(L"./Assets/Textures/TexturesCom_BrownBricks_albedo_M.tif", Texture2DType::ALBEDO);
	exitGame.text = L"Exit Game";
	exitGame.onPressed = &UIPanelMenu::ExitGame;
}

UIPanelMenu::~UIPanelMenu()
{
}

void UIPanelMenu::Draw(SpriteBatch * const spriteBatch, const std::unordered_map<const char*, SpriteFont*>& fontMap)
{
	spriteBatch->Draw(startGame.texture->GetSRV(), startGame.rectangle);
	fontMap.at("arial")->DrawString(spriteBatch, startGame.text.c_str(), XMFLOAT2(startGame.rectangle.x, startGame.rectangle.y));

	spriteBatch->Draw(exitGame.texture->GetSRV(), exitGame.rectangle);
	fontMap.at("arial")->DrawString(spriteBatch, exitGame.text.c_str(), XMFLOAT2(exitGame.rectangle.x, exitGame.rectangle.y));
}

void UIPanelMenu::MousePressed(float x, float y)
{
	if(startGame.CheckIsPressed(x, y))
		(this->*startGame.onPressed)();
	if (exitGame.CheckIsPressed(x, y))
		(this->*exitGame.onPressed)();
}

void UIPanelMenu::PlayGame()
{
	stateManager->SetState(GameState::GAME);
}

void UIPanelMenu::ExitGame()
{
	exit(EXIT_SUCCESS);
}
