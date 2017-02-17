/***************************************************************
|	File:		MainMenuState.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "OptionsState.h"
#include "InstructionsState.h"
#include "CreditsState.h"

#include <fstream>
using namespace std;

/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ MainMenuState* MainMenuState::GetInstance( void )
{
	static MainMenuState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
//	- reset the game & load resources
/*virtual*/ void MainMenuState::Enter( void )
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	//m_nCursor = 0;

	
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 92, 148, 252 });	// sky blue

	fstream fin;
	fin.open("options.txt", ios_base::in);

	if (fin.is_open())
	{
		while (true)
		{
			fin >> m_musVol >> m_sfxVol;

			fin.ignore(INT_MAX, '\n'); //or \t, etc...whatever you need to ignore thru

			if (fin.eof()) //only returns true after eof has been read
				break;
		}

		fin.close();
	}

	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, m_musVol);
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, m_sfxVol);
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void MainMenuState::Exit( void )
{
}


/**************************************************************/
// Update
//	- handle input & update entites
/*virtual*/ bool MainMenuState::Update( float elapsedTime )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit
	if( pInput->IsKeyPressed( SGD::Key::Escape ) == true )
		m_nCursor = 4;	// quit game

	// Move cursor
	if( pInput->IsKeyPressed( SGD::Key::Down ) == true )
	{
		++m_nCursor;

		// Wrap around
		if( m_nCursor > 4 )
			m_nCursor = 0;
	}
	else if( pInput->IsKeyPressed( SGD::Key::Up ) == true )
	{
		--m_nCursor;

		// Wrap around
		if( m_nCursor < 0 )
			m_nCursor = 4;
	}


	// Select an option?
	if( pInput->IsKeyPressed( SGD::Key::Enter ) == true && pInput->IsKeyDown(SGD::Key::Alt) == false )
	{
		switch (m_nCursor)
		{
		case 0: // PLAY
			// ChangeState is VERY VOLATILE!!!
			//	- can only be safely called by a game state's
			//	 Update or Render methods!
			Game::GetInstance()->ChangeState(GameplayState::GetInstance());

			// Exit this state immediately
			return true;	// keep playing in the new state
			break;
		case 1:
			Game::GetInstance()->ChangeState(OptionsState::GetInstance());

			return true;
			break;
		case 2:
			Game::GetInstance()->ChangeState(InstructionsState::GetInstance());

			return true;
			break;
		case 3:
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());

			return true;
			break;
		case 4: // EXIT
			return false; // quit the game
			break;
		}
	}

	return true;	// keep playing
}


/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void MainMenuState::Render( float elapsedTime )
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


	// Display the game title centered at 4x scale
	pFont->Draw( "Super Mario Clone", { (width - (17 * 7 * 4.0f))/2, 50 }, 
				 4.0f, { 255, 255, 255 } );

	// Display the menu options centered at 1x scale
	pFont->Draw( "Play", { (width - (4 * 7 * 3.0f))/2, 300 }, 
				 3.0f, {255, 0, 0} );
	pFont->Draw("Options", { (width - (7 * 7 * 3.0f)) / 2, 350 },
		3.0f, { 255, 0, 0 });
	pFont->Draw("Instructions", { (width - (12 * 7 * 3.0f)) / 2, 400 },
		3.0f, { 255, 0, 0 });
	pFont->Draw("Credits", { (width - (7 * 7 * 3.0f)) / 2, 450 },
		3.0f, { 255, 0, 0 });
	pFont->Draw( "Exit", { (width - (4 * 7 * 3.0f))/2, 500 }, 
				 3.0f, {255, 0, 0} );

	//draw the selected menu item in green
	switch (m_nCursor)
	{
	case 0:
		pFont->Draw("Play", { (width - (4 * 7 * 3.0f)) / 2, 300 },
			3.0f, { 0, 255, 0 });
		break;
	case 1:
		pFont->Draw("Options", { (width - (7 * 7 * 3.0f)) / 2, 350 },
			3.0f, { 0, 255, 0 });
		break;
	case 2:
		pFont->Draw("Instructions", { (width - (12 * 7 * 3.0f)) / 2, 400 },
			3.0f, { 0, 255, 0 });
		break;
	case 3:
		pFont->Draw("Credits", { (width - (7 * 7 * 3.0f)) / 2, 450 },
			3.0f, { 0, 255, 0 });
		break;
	case 4:
		pFont->Draw("Exit", { (width - (4 * 7 * 3.0f)) / 2, 500 },
			3.0f, { 0, 255, 0 });
		break;
	}

	// Display the cursor next to the option
	/*pFont->Draw( "*            *", { (width - (14 * 7 * 3.0f))/2, 300.0f + 50*m_nCursor }, 
				 3.0f, {0, 255, 0} );*/

}
