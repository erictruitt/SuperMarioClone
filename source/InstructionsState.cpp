/***************************************************************
|	File:		InstructionsState.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "InstructionsState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_InputManager.h"

/*static*/ InstructionsState* InstructionsState::GetInstance(void)
{
	static InstructionsState s_Instance;

	return &s_Instance;
}

/*virtual*/ void InstructionsState::Enter(void) /*override*/
{

}

/*virtual*/ void InstructionsState::Exit(void) /*override*/
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_mario);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_goomba);
}

/*virtual*/ bool InstructionsState::Update(float elapsedTime) /*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit to Main Menu
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		return true;	// keep playing in the new state
	}

	return true;
}

/*virtual*/ void InstructionsState::Render(float elapsedTime) /*override*/
{
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	pFont->Draw("Objective", { (width - (9 * 7 * 3.0f)) / 2, 50 },
		3.0f, { 255, 0, 0 });

	pFont->Draw("Get to the end of the level before time runs out", { (width - (49 * 7 * 2.0f)) / 2, 100 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("Controls", { (width - (8 * 7 * 3.0f)) / 2, 150 },
		3.0f, { 255, 0, 0 });

	pFont->Draw("Movement - Left and Right Arrow Keys", { (width - (36 * 7 * 2.0f)) / 2, 200 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("Jump - Spacebar", { (width - (15 * 7 * 2.0f)) / 2, 250 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("Shoot - alt", { (width - (11 * 7 * 2.0f)) / 2, 300 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("Player", { (width - (8 * 7 * 3.0f)) / 4, 400 },
		3.0f, { 255, 0, 0 });

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_mario,
	{ (width - (7 * 3.0f)) / 4, 450 },
	{}, {}, 
	{255, 255, 255},
	{ 3.0f, 3.0f });

	pFont->Draw("Enemies", { (width - (8 * 7 * 3.0f)) / 1.25f , 400 },
		3.0f, { 255, 0, 0 });

	SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_goomba,
	{ width / 1.45f, 450 },
	{ 0, 0, 16, 16 },
	{}, {},
	{ 255 , 255, 255 },
	{ 3.0f, 3.0f });
}
