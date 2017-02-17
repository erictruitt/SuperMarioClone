/***************************************************************
|	File:		Game.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Game.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "BitmapFont.h"
#include "IGameState.h"
#include "MainMenuState.h"

#include <ctime>
#include <cstdlib>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/**************************************************************/
// Singleton
//	- instantiate the static member
/*static*/ Game* Game::s_pInstance = nullptr;

// GetInstance

//	- allocate the ONE instance & return it
/*static*/ Game* Game::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new Game;
	return s_pInstance;
}

// DeleteInstance
//	- deallocate the ONE instance
/*static*/ void Game::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/**************************************************************/
// Initialize
//	- initialize the SGD wrappers
//	- enter the Main Menu state
bool Game::Initialize(float width, float height)
{
	// Seed First!
	srand((unsigned int)time(nullptr));
	rand();

	// Create local pointers to the singletons
	SGD::InputManager*		pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio = SGD::AudioManager::GetInstance();

	// Initialize the wrappers
	if (pAudio->Initialize() == false
		|| pGraphics->Initialize(false) == false
		|| pInput->Initialize() == false)
		return false;

	// Store the size parameters
	m_fScreenWidth = width;
	m_fScreenHeight = height;

	// Allocate & initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize();

	// Start the game in the Main Menu state
	ChangeState(MainMenuState::GetInstance());

	// Store the current time (in milliseconds)
	m_ulGameTime = GetTickCount();

	return true;	// success!
}


/**************************************************************/
// Update
//	- update the SGD wrappers
//	- run the current state
int Game::Update(void)
{
	SGD::InputManager*		pInput = SGD::InputManager::GetInstance();

	// Update the wrappers
	if (SGD::AudioManager::GetInstance()->Update() == false
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false)
		return -10;		// exit FAILURE!

	// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();					// current time in milliseconds
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;	// convert to fraction of a second
	m_ulGameTime = now;									// store the current time

	// Cap the elapsed time to 1/8th of a second
	if (elapsedTime > 0.125f)
		elapsedTime = 0.125f;

	// Let the current state handle input
	if (m_pCurrState->Update(elapsedTime) == false)
		return 1;	// exit success!

	// Let the current state render
	m_pCurrState->Render(elapsedTime);

	//go fullscreen
	if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyPressed(SGD::Key::Enter) == true)
	{
		windowed = !windowed;
		SGD::GraphicsManager::GetInstance()->Resize({ m_fScreenWidth, m_fScreenHeight }, windowed);
		return 0;
	}

	return 0;		// keep playing!
}


/**************************************************************/
// Terminate
//	- exit the current state
//	- terminate the SGD wrappers
void Game::Terminate(void)
{
	// Exit the current state
	ChangeState(nullptr);

	// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;

	// Terminate the core SGD wrappers
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();

	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();

	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();
}


/**************************************************************/
// ChangeState
//	- exit the current state to release resources
//	- enter the new state to allocate resources
//	- DANGER! Exiting the current state can CRASH the program!
//	  The state can ONLY be exited from the
//	  Input, Update, and Render methods!!!
void Game::ChangeState(IGameState* pNewState)
{
	// Exit the old state
	if (m_pCurrState != nullptr)
		m_pCurrState->Exit();

	// Store the new state
	m_pCurrState = pNewState;

	// Enter the new state
	if (m_pCurrState != nullptr)
		m_pCurrState->Enter();
}