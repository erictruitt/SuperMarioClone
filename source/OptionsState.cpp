/***************************************************************
|	File:		OptionsState.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "OptionsState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "Mario.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include <fstream>
using namespace std;

/*static*/ OptionsState* OptionsState::GetInstance(void)
{
	static OptionsState s_Instance;

	return &s_Instance;
}

/*virtual*/ void	OptionsState::Enter(void)				/*override*/
{
	m_background = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_BACKGROUND.xwm");
	m_sfx = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_jump-small.wav");
}

/*virtual*/ void	OptionsState::Exit(void)				/*override*/
{
	fstream fout;
	fout.open("options.txt", ios_base::out | ios_base::trunc); //ios_base::app will append to the end of the file, trunc erases everything and starts fresh

	if (fout.is_open())
	{
		fout << m_musVol << '\t' << m_sfxVol;

		fout.close();
	}
	fout.clear();

	SGD::AudioManager::GetInstance()->UnloadAudio(m_background);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_sfx);

}

/*virtual*/ bool	OptionsState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to quit to Main Menu
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		return true;	// keep playing in the new state
	}

	// Move cursor
	if (pInput->IsKeyPressed(SGD::Key::Down) == true)
	{
		++m_nCursor;

		// Wrap around
		if (m_nCursor > 1)
			m_nCursor = 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true)
	{
		--m_nCursor;

		// Wrap around
		if (m_nCursor < 0)
			m_nCursor = 1;
	}

	switch (m_nCursor)
	{
	case 0:
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_background) == false)
			SGD::AudioManager::GetInstance()->PlayAudio(m_background);

		if (pInput->IsKeyPressed(SGD::Key::Right) == true)
		{
			m_musVol += 5;

			if (m_musVol > 100)
				m_musVol = 100;

		}
		else if (pInput->IsKeyPressed(SGD::Key::Left) == true)
		{
			m_musVol -= 5;

			if (m_musVol < 0)
				m_musVol = 0;
		}

		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, m_musVol);

		break;
	case 1:
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_background) == true)
			SGD::AudioManager::GetInstance()->StopAudio(m_background);

		if (pInput->IsKeyPressed(SGD::Key::Right) == true)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_sfx);

			m_sfxVol += 5;

			if (m_sfxVol > 100)
				m_sfxVol = 100;

		}
		else if (pInput->IsKeyPressed(SGD::Key::Left) == true)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_sfx);

			m_sfxVol -= 5;

			if (m_sfxVol < 0)
				m_sfxVol = 0;
		}

		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, m_sfxVol);

		break;
	}

	return true;
}

/*virtual*/ void	OptionsState::Render(float elapsedTime)	/*override*/
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::OStringStream sfx, music;


	music << m_musVol;
	sfx << m_sfxVol;

	

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	pFont->Draw("Volume Control", { (width - (14 * 7 * 4.0f)) / 2, 50 },
		4.0f, { 255, 255, 255 });

	pFont->Draw("Music", { (width - (5 * 7 * 3.0f)) / 2, 300 },
		3.0f, { 255, 0, 0 });

	pGraphics->DrawString(music.str().c_str(),
	{ (width - 35) / 2, 350 },
	{ 255, 255, 255 });

	pFont->Draw("SFX", { (width - (3 * 7 * 3.0f)) / 2, 400 },
		3.0f, { 255, 0, 0 });

	pGraphics->DrawString(sfx.str().c_str(),
	{ (width - 35) / 2, 450 },
	{ 255, 255, 255 });

	// Display the cursor next to the option
	pFont->Draw( "*     *", { (width - (7 * 7 * 3.0f))/2, 300.0f + 100*m_nCursor },
	3.0f, {0, 255, 0} );
}
