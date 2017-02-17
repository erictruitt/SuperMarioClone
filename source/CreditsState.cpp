/***************************************************************
|	File:		CreditsState.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "CreditsState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

/*static*/ CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState s_Instance;

	return &s_Instance;
}

/*virtual*/ void CreditsState::Enter(void) /*override*/
{
	m_scrollControl = Game::GetInstance()->GetScreenHeight();
}

/*virtual*/ bool CreditsState::Update(float elapsedTime) /*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	scrollreturn = m_scrollControl + 350;

	// Press Escape to quit to Main Menu
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || ((int)scrollreturn == 0))
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		return true;	// keep playing in the new state
	}

	m_scrollControl -= 100 * elapsedTime;

	return true;
}

/*virtual*/ void CreditsState::Render(float elapsedTime) /*override*/
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	pFont->Draw("Developed by - Eric Truitt", { (width - (26 * 7 * 4.0f)) / 2, m_scrollControl },
		4.0f, { 255, 255, 255 });

	pFont->Draw("Structure of Game Design", { (width - (24 * 7 * 3.0f)) / 2, m_scrollControl + 250 },
		3.0f, { 255, 0, 0 });
	pFont->Draw("at Full Sail University", { (width - (23 * 7 * 3.0f)) / 2, m_scrollControl + 300 },
		3.0f, { 255, 0, 0 });
}
