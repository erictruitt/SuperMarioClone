/***************************************************************
|	File:		GameplayState.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	GameplayState class initializes & runs the game logic
***************************************************************/

#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "Map.h"
#include "Mario.h"
#include "BitmapFont.h"
#include "Goomba.h"
#include "CreditsState.h"
#include "SpecialMysteryBox.h"
#include "Mushroom.h"
#include "Flower.h"
#include "FireBall.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"

#include "Entity.h"
#include "EntityManager.h"

#include <cstdlib>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <fstream>
using namespace std;


/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ GameplayState* GameplayState::GetInstance(void)
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}


/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void GameplayState::Enter(void)	/*override*/
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 92, 148, 252 });	// sky blue

	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	//load & play music
	m_backgroundMus = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/EHT_BACKGROUND.xwm");
	SGD::AudioManager::GetInstance()->PlayAudio(m_backgroundMus, true);


	//Create Map, add to entity manager, release
	m_mapImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_TILESET.png");
	m_pMap = CreateMap();

	m_pEntities->AddEntity(m_pMap, 0);

	//Create Mario, add to entity manager, release
	m_marioImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_MARIO_STANDING.png");
	m_localMario = CreateMario();

	m_pEntities->AddEntity(m_localMario, 1);

	//load other Misc textures
	//m_specialBoxImg 
	//m_mushroomImg 
	m_mushroomImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_MUSHROOM.png");
	m_flowerImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_FLOWER.png");
	m_fireballImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_FIREBALL.png");

	//if the game was paused, load mario's variables as well as the world position
	if (m_gamePaused == true)
	{
		m_gamePaused = false;

		fstream fin;
		fin.open("pause.txt", ios_base::in);

		if (fin.is_open())
		{
			//declare any buffer variables needed to catch info being loaded in here
			float marioX, marioY, marioW, marioH, mapX, mapY, timer;
			int marioDam, marioState, score;

			while (true)
			{
				fin >> marioX >> marioY >> marioW >> marioH >> marioDam >> marioState >> timer >> score >> mapX >> mapY;

				//store buffer variables into game/program member variables
				m_localMario->SetPosition({ marioX, marioY });
				m_localMario->SetSize({ marioW, marioH });
				dynamic_cast<Mario*>(m_localMario)->SetDamageLeft(marioDam);
				dynamic_cast<Mario*>(m_localMario)->SetState(marioState);
				dynamic_cast<Mario*>(m_localMario)->SetTimer(timer);
				dynamic_cast<Mario*>(m_localMario)->SetScore(score);

				m_pMap->SetPosition({ mapX, mapY });

				fin.ignore(INT_MAX, '\n'); //or \t, etc...whatever you need to ignore thru

				if (fin.eof()) //only returns true after eof has been read
					break;
			}

			fin.close();
		}
	}

	m_pMap->Release();
	m_localMario->Release();
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void GameplayState::Exit(void)	/*override*/
{
	// Deallocate the Entity Manager
	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;

	//unload music
	SGD::AudioManager::GetInstance()->UnloadAudio(m_backgroundMus);

	//unload textures
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_mapImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_marioImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_mushroomImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_flowerImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_fireballImg);

	// Terminate & deallocate the SGD wrappers
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();

	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();
}


/**************************************************************/
// Update
//	- handle input & update entities
/*virtual*/ bool GameplayState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	//////////////////////////////////////////////////////////////////////////////////////////
	Entity* mario = GameplayState::GetInstance()->GetMario();

	if (dynamic_cast<Mario*>(mario)->GetState() == Mario::MARIOSTATE::MARIO_LOST &&
		SGD::AudioManager::GetInstance()->IsAudioPlaying(dynamic_cast<Mario*>(mario)->GetGameOverMus()) == false)
	{
		Game::GetInstance()->ChangeState(CreditsState::GetInstance());

		////// Exit this state immediately

		return true;	// keep playing in the new state
	}
	//////////////////////////////////////////////////////////////////////////////////////////

	if (dynamic_cast<Mario*>(mario)->GetState() == Mario::MARIOSTATE::MARIO_WIN && SGD::AudioManager::GetInstance()->IsAudioPlaying(dynamic_cast<Mario*>(mario)->GetWinningMus()) == false)
	{
		Game::GetInstance()->ChangeState(CreditsState::GetInstance());

		////// Exit this state immediately

		return true;	// keep playing in the new state
	}


	// Press Escape to return to Main Menu
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true && m_gamePaused == false && dynamic_cast<Mario*>(mario)->GetState() != Mario::MARIOSTATE::MARIO_LOST)
	{
		m_gamePaused = true;
		m_justpaused = true;
	}

	//make sure the esc key is released
	if (pInput->IsKeyReleased(SGD::Key::Escape) && m_gamePaused == true)
	{
		m_justpaused = false;
	}

	//unpause the game with esc
	if (pInput->IsKeyPressed(SGD::Key::Escape) && m_gamePaused == true && m_justpaused == false)
	{
		m_gamePaused = false;
	}

	if (pInput->IsKeyPressed(SGD::Key::Down) == true && m_gamePaused)
	{
		++m_nCursor;

		// Wrap around
		if (m_nCursor > 1)
			m_nCursor = 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true && m_gamePaused)
	{
		--m_nCursor;

		// Wrap around
		if (m_nCursor < 0)
			m_nCursor = 1;
	}

	// pause menu selection
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true && m_gamePaused)
	{
		switch (m_nCursor)
		{
		case 0:
			m_gamePaused = false;
			break;
		case 1:

			//save any necessary data (position, state, damageleft, map position)
			//////////////////////////////////////////////////////////////////////////////////////////
			fstream fout;
			fout.open("pause.txt", ios_base::out | ios_base::trunc);

			if (fout.is_open())
			{
				fout << m_localMario->GetPosition().x << '\t' << m_localMario->GetPosition().y << '\t'
					<< m_localMario->GetSize().width << '\t' << m_localMario->GetSize().height << '\t'
					<< dynamic_cast<Mario*>(m_localMario)->GetDamageLeft() << '\t' << dynamic_cast<Mario*>(m_localMario)->GetState()
					<< '\t' << dynamic_cast<Mario*>(m_localMario)->GetTimer() << '\t' << dynamic_cast<Mario*>(m_localMario)->GetScore()
					<< '\t' << m_pMap->GetPosition().x << '\t' << m_pMap->GetPosition().y;

				fout.close();
			}
			fout.clear();
			/////////////////////////////////////////////////////////////////////////////////////////////


			//// ChangeState is VERY VOLATILE!!!
			////	- can only be safely called by a game state's
			////	  Update or Render methods!
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

			//// Exit this state immediately
			return true;	// keep playing in the new state
			break;
		}
	}

	if (m_gamePaused == false)
	{
		// Update the entities
		m_pEntities->UpdateAll(elapsedTime);
		m_pEntities->CheckCollisions(1, 1);
		//m_pEntities->CheckCollisions(2, 1);
		m_pEntities->CheckCollisions(1, 2);
	}

	//m_cameraWidth = m_localMario->GetPosition().x - ((Game::GetInstance()->GetScreenWidth() * 0.5f) / 3);
	//m_cameraHeight = m_localMario->GetPosition().y - ((Game::GetInstance()->GetScreenHeight() * 0.5f) / 3);

	//unpause game with esc key
	//if (pInput->IsKeyPressed(SGD::Key::Escape) == true && m_gamePaused == true)
	//m_gamePaused = false;

	// Process the events & messages
	SGD::EventManager::GetInstance()->Update();
	SGD::MessageManager::GetInstance()->Update();

	return true;	// keep playing
}

/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render(float elapsedTime)	/*override*/
{
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	Entity* mario = GameplayState::GetInstance()->GetMario();

	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();

	m_pEntities->RenderAll();

	if (dynamic_cast<Mario*>(mario)->GetState() == Mario::MARIOSTATE::MARIO_LOST)
	{
		//set background to black
		SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });

		pFont->Draw("G a m e  O v e r", { (width - (16 * 7 * 4.0f)) / 2, (height * 0.5f) - 64 },
			4.0f, { 255, 255, 255 });
	}

	if (dynamic_cast<Mario*>(mario)->GetState() == Mario::MARIOSTATE::MARIO_WIN)
	{
		pFont->Draw("Y o u  W o n !", { (width - (14 * 7 * 4.0f)) / 2, (height * 0.5f) - 64 },
			4.0f, { 255, 255, 255 });
	}

	//render the HUD
	pFont->Draw("Mario", { 36, 18 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("World", { 5 * 5 * 2.0f * 10, 18 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("Get to the castle", { 180, 18 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("before time runs out!", { 155, 43 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("1-1", { 5 * 5 * 2.0f * 10 + 16, 38 },
		2.0f, { 255, 255, 255 });

	pFont->Draw("Time", { 5 * 5 * 2.0f * 13, 18 },
		2.0f, { 255, 255, 255 });

	//pause menu
	if (m_gamePaused)
	{
		pFont->Draw("Pause", { (width - (5 * 7 * 4.0f)) / 2, (height * 0.5f) - 64 },
			4.0f, { 255, 255, 255 });

		// Display the menu options centered at 1x scale
		pFont->Draw("Resume", { (width - (6 * 7 * 3.0f)) / 2, (height * 0.5f) },
			3.0f, { 255, 0, 0 });
		pFont->Draw("Exit", { (width - (4 * 7 * 3.0f)) / 2, (height * 0.5f) + 50 },
			3.0f, { 255, 0, 0 });

		// Display the cursor next to the option
		pFont->Draw("*      *", { (width - (8 * 7 * 3.0f)) / 2, (height * 0.5f) + 50 * m_nCursor },
			3.0f, { 0, 255, 0 });
	}
}

/**************************************************************/
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD
//		- does NOT have invoking object!!!
//		- must use singleton to access members
/*static*/ void GameplayState::MessageProc(const SGD::Message* pMsg)
{
	/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch (pMsg->GetMessageID())
	{
	case MessageID::MSG_UNKNOWN:
	{
		OutputDebugStringW(L"Game::MessageProc - unknown message id\n");
	}
		break;
	case MessageID::MSG_DESTROY_ENTITY:
		// Downcast to the actual message type
		const DestroyEntityMessage* pDestroyMsg =
			dynamic_cast<const DestroyEntityMessage*>(pMsg);

		assert(pDestroyMsg != nullptr
			&& "Game::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

		Entity* ptr = pDestroyMsg->GetEntity();

		// Use the Game singleton to access members!
		GameplayState::GetInstance()->m_pEntities->RemoveEntity(ptr);
		break;

	}

	/* Restore previous warning levels */
#pragma warning( pop )
}

Entity* GameplayState::CreateMap(void)
{
	Map* newMap = new Map;

	newMap->SetImage(m_mapImg);
	newMap->LoadWorld();

	return newMap;
}

Entity* GameplayState::CreateMario(void)
{
	Mario* mario = new Mario;
	mario->SetImage(m_marioImg);
	mario->SetPosition({ 120.0f, 526.0f });
	mario->SetSize({ 36, 48 });

	return mario;
}

void GameplayState::CreateGoomba(int _x, int _y)
{
	Goomba* goomba = new Goomba;

	goomba->SetPosition({ _x * 16.0f * 3, _y * 16.0f * 3 });
	goomba->SetSize({ 48, 48 });

	m_pEntities->AddEntity(goomba, 1);
	goomba->Release();
}

void GameplayState::CreateSpecialBox(int _x, int _y)
{
	SpecialMysteryBox* newBox = new SpecialMysteryBox;

	//newBox->SetImage(m_specialBoxImg);
	newBox->SetPosition({ _x * 16.0f * 3, _y * 16.0f * 3 });
	newBox->SetSize({ 48, 48 });

	m_pEntities->AddEntity(newBox, 2);
	newBox->Release();
}

void GameplayState::CreatePowerUp(int _damageLeft, float _x, float _y)
{
	//create a mushroom
	if (_damageLeft == 1)
	{
		Mushroom* mushroom = new Mushroom;
		mushroom->SetImage(m_mushroomImg);
		mushroom->SetSize({ 48, 48 });
		mushroom->SetPosition({ _x, _y - (16 * 3) });

		m_pEntities->AddEntity(mushroom, 2);
		mushroom->Release();
	}
	//create a flower
	else if (_damageLeft == 2)
	{
		Flower* flower = new Flower;
		flower->SetImage(m_flowerImg);
		flower->SetSize({ 48, 48 });
		flower->SetPosition({ _x, _y - (16 * 3) });

		m_pEntities->AddEntity(flower, 2);
		flower->Release();
	}
}

void GameplayState::CreateFireBall(float _x, float _y)
{
	FireBall* fireball = new FireBall;
	fireball->SetImage(m_fireballImg);
	fireball->SetSize({ 24, 24 });
	fireball->SetPosition({_x , _y});
	fireball->SetVelocity({ 500, -500 });

	m_pEntities->AddEntity(fireball, 2);
	fireball->Release();
}

