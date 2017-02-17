/***************************************************************
|	File:		Mario.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "Mario.h"

#include "GameplayState.h"
#include "Map.h"
#include "AnchorPointAnimation.h"
#include "BitmapFont.h"
#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

#define GRAVITY 2500.0f

Mario::Mario() : SGD::Listener(this)
{
	if (m_walkingAnim == nullptr)
	{
		m_walkingAnim = new AnchorPointAnimation;
		m_walkingAnim->Initialize(L"resource/graphics/EHT_MARIO_WALKING.png",
		{ 0, 0, 44, 47 }, { 57, 0, 89, 47 }, { 102, 0, 140, 47 },
 		{ 22, 23.5 }, { 16, 23.5 }, { 19, 23.5 });
	}

	if (m_BigWalkingAnim == nullptr)
	{
		m_BigWalkingAnim = new AnchorPointAnimation;
		m_BigWalkingAnim->Initialize(L"resource/graphics/EHT_BIG_MARIO_WALKING.png",
		{ 0, 0, 47, 95 }, { 51, 0, 92, 95 }, { 96, 0, 143, 95 },
		{ 23.5, 47.5 }, { 20.5, 47.5 }, { 23.5, 47.5 });
	}

	if (m_FireWalkingAnim == nullptr)
	{
		m_FireWalkingAnim = new AnchorPointAnimation;
		m_FireWalkingAnim->Initialize(L"resource/graphics/EHT_FIRE_MARIO_WALKING.png",
		{ 0, 0, 47, 95 }, { 51, 0, 92, 95 }, { 96, 0, 143, 95 },
		{ 23.5, 47.5 }, { 20.5, 47.5 }, { 23.5, 47.5 });
	}

	SGD::Listener::RegisterForEvent("SCORE");
}

/*virtual*/ Mario::~Mario()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_deadImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_jumpingImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_BigJumpingImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_BigMarioImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_FireJumpingImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_FireMarioImg);

	SGD::AudioManager::GetInstance()->UnloadAudio(m_smallJumpSfx);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_gameOverMus);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_loseLifeMus);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_levelCompleteMus);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_powerUpSfx);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_fireBallSfx);

	if (m_walkingAnim != nullptr)
	{
		m_walkingAnim->Terminate();
		delete m_walkingAnim;
		m_walkingAnim = nullptr;
	}

	if (m_BigWalkingAnim != nullptr)
	{
		m_BigWalkingAnim->Terminate();
		delete m_BigWalkingAnim;
		m_BigWalkingAnim = nullptr;
	}

	if (m_FireWalkingAnim != nullptr)
	{
		m_FireWalkingAnim->Terminate();
		delete m_FireWalkingAnim;
		m_FireWalkingAnim = nullptr;
	}
}

/*virtual*/ void Mario::Update(float elapsedTime) /*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	Entity* map = GameplayState::GetInstance()->GetMap();

	switch (m_marioState)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MARIO_ALIVE:

		//make sure the timer hasn't run out and mario hasn't fallen off the ground && he has damage left
		if (m_gameTimer < 0.0f || GetPosition().y + GetSize().height >= Game::GetInstance()->GetScreenHeight() || m_damageLeft == 0)
		{
			//game over, set all appropriate variables
			m_marioState = MARIO_DEAD;
			SGD::AudioManager::GetInstance()->StopAudio(GameplayState::GetInstance()->GetBackgroundMusic());
			SGD::AudioManager::GetInstance()->PlayAudio(m_loseLifeMus);
			SetImage(m_deadImg);
			SetSize({ 42, 42 });
			SetVelocity({ GetVelocity().x, -1000.0f });

			return;
		}
		else if (dynamic_cast<Map*>(map)->ReturnWin(GetPosition().x + (GetSize().width * 0.5f), GetPosition().y + (GetSize().height * 0.5f)))
		{
			m_marioState = MARIO_WIN;
			SGD::AudioManager::GetInstance()->StopAudio(GameplayState::GetInstance()->GetBackgroundMusic());
			SGD::AudioManager::GetInstance()->PlayAudio(m_levelCompleteMus);
			return;
		}
		else
			m_gameTimer -= elapsedTime;

		//move mario
		if (pInput->IsKeyDown(SGD::Key::Right) || pInput->IsKeyPressed(SGD::Key::Right))
		{
			if (m_damageLeft == 1)
			{
				if (m_walkingAnim != nullptr && m_walkingAnim->IsPlaying() == true)
					m_walkingAnim->Update(elapsedTime);
			}
			else if (m_damageLeft == 2)
			{
				if (m_BigWalkingAnim != nullptr && m_BigWalkingAnim->IsPlaying() == true)
					m_BigWalkingAnim->Update(elapsedTime);
			}
			else if (m_damageLeft == 3)
			{
				if (m_FireWalkingAnim != nullptr && m_FireWalkingAnim->IsPlaying() == true)
					m_FireWalkingAnim->Update(elapsedTime);
			}

			//only move if the space is OK to move onto
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width + (256 * elapsedTime), GetPosition().y) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width + (256 * elapsedTime), GetPosition().y + GetSize().height) == true)
				SetPosition({ GetPosition().x + (256 * elapsedTime), GetPosition().y });
		}
		else if (pInput->IsKeyDown(SGD::Key::Left) || pInput->IsKeyPressed(SGD::Key::Left))
		{
			if (m_damageLeft == 1)
			{
				if (m_walkingAnim != nullptr && m_walkingAnim->IsPlaying() == true)
					m_walkingAnim->Update(elapsedTime);
			}
			else if (m_damageLeft == 2)
			{
				if (m_BigWalkingAnim != nullptr && m_BigWalkingAnim->IsPlaying() == true)
					m_BigWalkingAnim->Update(elapsedTime);
			}
			else if (m_damageLeft == 3)
			{
				if (m_FireWalkingAnim != nullptr && m_FireWalkingAnim->IsPlaying() == true)
					m_FireWalkingAnim->Update(elapsedTime);
			}

			//only move if the space is OK to move onto
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - (256 * elapsedTime), GetPosition().y) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - (256 * elapsedTime), GetPosition().y + GetSize().height) == true)
				SetPosition({ GetPosition().x - (256 * elapsedTime), GetPosition().y });
		}

		if (pInput->IsKeyPressed(SGD::Key::Space))
			Jump();

		//don't let him move further than half the screen width, or off the left side of the screen
		if (GetPosition().x > (Game::GetInstance()->GetScreenWidth() - (GetSize().width * 3)) * 0.5f)
			SetPosition({ (Game::GetInstance()->GetScreenWidth() - (GetSize().width * 3)) * 0.5f, GetPosition().y });
		else if (GetPosition().x < 0)
			SetPosition({ 0.0f, GetPosition().y });

		if (m_damageLeft == 3 && pInput->IsKeyPressed(SGD::Key::Z))
		{
			//create a fireball
			GameplayState::GetInstance()->CreateFireBall(GetPosition().x + GetSize().width, GetPosition().y + (GetSize().width * 0.5f));

			//SetPosition({ GetPosition().x, GetPosition().y });
			SGD::AudioManager::GetInstance()->PlayAudio(m_fireBallSfx);
		}

		//update his movements
		Entity::Update(elapsedTime);

		if (m_isJumping == true)
		{
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x, GetPosition().y + GetSize().height) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width, GetPosition().y + GetSize().height) == true)
			{
				SetVelocity({ GetVelocity().x, GetVelocity().y + GRAVITY * elapsedTime });
			}
			else
			{
				SetVelocity({ 0, 0 });
				m_isJumping = false;
			}
		}

		//this keeps mario from falling through objects he's standing on
		while ((dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x, GetPosition().y + GetSize().height) == false ||
			dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width, GetPosition().y + GetSize().height) == false) &&
			m_isJumping == false)
		{
			SetPosition({ GetPosition().x, GetPosition().y - 0.1f });
		}

		//this makes mario fall when he steps off an object he's standing on
		if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x, GetPosition().y + GetSize().height) == true &&
			dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width, GetPosition().y + GetSize().height) == true &&
			m_isJumping == false)
		{
			SetVelocity({ GetVelocity().x, 1000.0f });
		}

		//this makes mario fall to the ground if he hits something with his head
		if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x, GetPosition().y) == false ||
			dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width, GetPosition().y) == false /*&&
																											GetPosition().y < 0*/)
		{
			SetVelocity({ GetVelocity().x, 1000.0f });
		}

		break;
		//case MARIO_FIRE_POWER:
		//	break;
		//case MARIO_STAR_POWER:
		//	break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MARIO_DEAD:

		m_deathTimer -= elapsedTime;
		if (m_deathTimer < 0.0f)
		{
			SetVelocity({ GetVelocity().x, GetVelocity().y + GRAVITY * elapsedTime });
			Entity::Update(elapsedTime);

			if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_loseLifeMus) == false)
			{
				m_marioState = MARIO_LOST;
				SGD::AudioManager::GetInstance()->PlayAudio(m_gameOverMus);
			}
		}

		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MARIO_WIN:

		//if mario isn't at the castle door then make him move to it
		if (dynamic_cast<Map*>(map)->CastleDoor(GetPosition().x, GetPosition().y) == false)
		{
			//make mario walk to the castle
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width + (256 * elapsedTime), GetPosition().y) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width + (256 * elapsedTime), GetPosition().y + GetSize().height) == true &&
				m_isJumping == false)
			{

				if (m_damageLeft == 1)
				{
					if (m_walkingAnim != nullptr && m_walkingAnim->IsPlaying() == true)
						m_walkingAnim->Update(elapsedTime);
				}
				else if (m_damageLeft == 2)
				{
					if (m_BigWalkingAnim != nullptr && m_BigWalkingAnim->IsPlaying() == true)
						m_BigWalkingAnim->Update(elapsedTime);
				}
				else if (m_damageLeft == 3)
				{
					if (m_FireWalkingAnim != nullptr && m_FireWalkingAnim->IsPlaying() == true)
						m_FireWalkingAnim->Update(elapsedTime);
				}

				SetPosition({ GetPosition().x + (128 * elapsedTime), GetPosition().y });
			}

			//pull mario down the flag pole
			if (m_isJumping == true)
			{
				if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x, GetPosition().y + GetSize().height) == true &&
					dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width, GetPosition().y + GetSize().height) == true)
				{
					SetVelocity({ GetVelocity().x, GetVelocity().y + GRAVITY * elapsedTime });
				}
				else
				{
					SetVelocity({ 0, 0 });
					m_isJumping = false;
				}
			}

			Entity::Update(elapsedTime);

			//this keeps mario from falling through objects he's standing on
			while ((dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x, GetPosition().y + GetSize().height) == false ||
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width, GetPosition().y + GetSize().height) == false) &&
				m_isJumping == false)
			{
				SetPosition({ GetPosition().x, GetPosition().y - 0.1f });
			}

			//this makes mario fall when he steps off an object he's standing on
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x, GetPosition().y + GetSize().height) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width, GetPosition().y + GetSize().height) == true &&
				m_isJumping == false)
			{
				SetVelocity({ GetVelocity().x, 1000.0f });
			}

		}

		break;
	}
}

/*virtual*/ void Mario::Render(void) /*override*/
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::OStringStream hud_score, /*hud_coins,*/ hud_time;
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	//Draw the hud
	hud_score << m_score;
	//hud_coins << m_coins;
	hud_time << (int)m_gameTimer;

	pGraphics->DrawString(hud_score.str().c_str(),
	{ 36, 38 },
	{ 255, 255, 255 });

	//pGraphics->DrawString(hud_coins.str().c_str(),
	//{ 136, 38 },
	//{ 255, 255, 255 });

	pGraphics->DrawString(hud_time.str().c_str(),
	{ 670, 38 },
	{ 255, 255, 255 });

	//if mario is at the castle door then return, we don't need to render him
	if (dynamic_cast<Map*>(GameplayState::GetInstance()->GetMap())->CastleDoor(GetPosition().x, GetPosition().y) == true)
		return;

	//Draw Mario
	//jumping
	if (m_isJumping  && m_marioState != MARIO_DEAD)
	{
		//small mario
		if (m_damageLeft == 1)
		{
			if (pInput->IsKeyDown(SGD::Key::Left) || pInput->IsKeyPressed(SGD::Key::Left))
			{
				pGraphics->DrawTexture(m_jumpingImg,
				{ GetPosition().x + (GetSize().width * 1.5f), GetPosition().y },
				{}, {}, {},
				{ -3.0f, 3.0f });
			}
			else if (pInput->IsKeyDown(SGD::Key::Right) || pInput->IsKeyPressed(SGD::Key::Right) || true)
			{
				pGraphics->DrawTexture(m_jumpingImg,
					GetPosition(),
					{}, {}, {},
					{ 3.0f, 3.0f });
			}
		}
		//big mario
		else if (m_damageLeft == 2)
		{
			if (pInput->IsKeyDown(SGD::Key::Left) || pInput->IsKeyPressed(SGD::Key::Left))
			{
				pGraphics->DrawTexture(m_BigJumpingImg,
				{ GetPosition().x + (GetSize().width * 1.5f), GetPosition().y },
				{}, {}, {},
				{ -3.0f, 3.0f });
			}
			else if (pInput->IsKeyDown(SGD::Key::Right) || pInput->IsKeyPressed(SGD::Key::Right) || true)
			{
				pGraphics->DrawTexture(m_BigJumpingImg,
					GetPosition(),
					{}, {}, {},
					{ 3.0f, 3.0f });
			}
		}
		//fire mario
		else if (m_damageLeft == 3)
		{
			if (pInput->IsKeyDown(SGD::Key::Left) || pInput->IsKeyPressed(SGD::Key::Left))
			{
				pGraphics->DrawTexture(m_FireJumpingImg,
				{ GetPosition().x + (GetSize().width * 1.5f), GetPosition().y },
				{}, {}, {},
				{ -3.0f, 3.0f });
			}
			else if (pInput->IsKeyDown(SGD::Key::Right) || pInput->IsKeyPressed(SGD::Key::Right) || true)
			{
				pGraphics->DrawTexture(m_FireJumpingImg,
					GetPosition(),
					{}, {}, {},
					{ 3.0f, 3.0f });
			}
		}

	}
	//looking left
	else if (pInput->IsKeyDown(SGD::Key::Left) || pInput->IsKeyPressed(SGD::Key::Left) && m_marioState != MARIO_DEAD)
	{
		if (m_damageLeft == 1)
		{
			if (m_walkingAnim->IsPlaying() == false)
				m_walkingAnim->Restart(false, 1.0f);

			if (m_walkingAnim != nullptr && m_walkingAnim->IsPlaying() == true)
			{
				m_walkingAnim->Render({ GetPosition().x + (GetSize().width * 0.5f), GetPosition().y + (GetSize().height * 0.5f) }, true, 1.0, { 255, 255, 255 });
			}
		}
		else if (m_damageLeft == 2)
		{
			if (m_BigWalkingAnim->IsPlaying() == false)
				m_BigWalkingAnim->Restart(false, 1.0f);

			if (m_BigWalkingAnim != nullptr && m_walkingAnim->IsPlaying() == true)
			{
				m_BigWalkingAnim->Render({ GetPosition().x + (GetSize().width * 0.5f), GetPosition().y + (GetSize().height * 0.5f) }, true, 1.0, { 255, 255, 255 });
			}
		}
		else if (m_damageLeft == 3)
		{
			if (m_FireWalkingAnim->IsPlaying() == false)
				m_FireWalkingAnim->Restart(false, 1.0f);

			if (m_FireWalkingAnim != nullptr && m_FireWalkingAnim->IsPlaying() == true)
			{
				m_FireWalkingAnim->Render({ GetPosition().x + (GetSize().width * 0.5f), GetPosition().y + (GetSize().height * 0.5f) }, true, 1.0, { 255, 255, 255 });
			}
		}
	}
	//looking right
	else if ((pInput->IsKeyDown(SGD::Key::Right) || pInput->IsKeyPressed(SGD::Key::Right) || m_marioState == MARIO_WIN) && m_marioState != MARIO_DEAD)
	{
		if (m_damageLeft == 1)
		{
			if (m_walkingAnim->IsPlaying() == false)
				m_walkingAnim->Restart(false, 1.0f);

			if (m_walkingAnim != nullptr && m_walkingAnim->IsPlaying() == true)
			{
				m_walkingAnim->Render({ GetPosition().x + (GetSize().width * 0.5f), GetPosition().y + (GetSize().height * 0.5f) }, false, 1.0, { 255, 255, 255 });
			}
		}
		else if (m_damageLeft == 2)
		{
			if (m_BigWalkingAnim->IsPlaying() == false)
				m_BigWalkingAnim->Restart(false, 1.0f);

			if (m_BigWalkingAnim != nullptr && m_walkingAnim->IsPlaying() == true)
			{
				m_BigWalkingAnim->Render({ GetPosition().x + (GetSize().width * 0.5f), GetPosition().y + (GetSize().height * 0.5f) }, false, 1.0, { 255, 255, 255 });
			}
		}
		else if (m_damageLeft == 3)
		{
			if (m_FireWalkingAnim->IsPlaying() == false)
				m_FireWalkingAnim->Restart(false, 1.0f);

			if (m_FireWalkingAnim != nullptr && m_FireWalkingAnim->IsPlaying() == true)
			{
				m_FireWalkingAnim->Render({ GetPosition().x + (GetSize().width * 0.5f), GetPosition().y + (GetSize().height * 0.5f) }, false, 1.0, { 255, 255, 255 });
			}
		}
	}
	else
	{
		if (m_damageLeft == 1 || m_marioState == MARIO_DEAD)
		{
			pGraphics->DrawTexture(m_hImage,
				GetPosition(),
				{}, {}, {},
				{ 3.0f, 3.0f });
		}
		else if (m_damageLeft == 2)
		{
			pGraphics->DrawTexture(m_BigMarioImg,
				GetPosition(),
				{}, {}, {},
				{ 3.0f, 3.0f });
		}
		else if (m_damageLeft == 3)
		{
			pGraphics->DrawTexture(m_FireMarioImg,
				GetPosition(),
				{}, {}, {},
				{ 3.0f, 3.0f });
		}
	}
}

/*virtual*/ SGD::Rectangle Mario::GetRect(void) const /*override*/
{
	return SGD::Rectangle(SGD::Point{ GetPosition().x, GetPosition().y }, GetSize());
}

/*virtual*/ void Mario::HandleCollision(const IEntity* pOther) /*override*/
{
	//if mario is already dead then return
	if (m_marioState == MARIO_DEAD)
		return;

	SGD::Rectangle marioRect = GetRect();
	SGD::Rectangle enemyRect = pOther->GetRect();
	SGD::Rectangle overlap = marioRect.ComputeIntersection(enemyRect);

	if (pOther->GetType() == ENT_GOOMBA || pOther->GetType() == ENT_KOOPA)
	{
		//did we collide from the top or bottom
		if (overlap.ComputeWidth() > overlap.ComputeHeight())
		{
			//jumped on enemy
			if (overlap.bottom == marioRect.bottom)
			{
				SGD::Event* stompevent = new SGD::Event{ "ENEMY_STOMPED", nullptr, this };
				SGD::EventManager::GetInstance()->QueueEvent(stompevent, pOther);

				SetVelocity({ GetVelocity().x, -500.0f });
			}
			//we were hit
			else
			{
				m_damageLeft--;
			}
		}
		//we were hit
		else
		{
			m_damageLeft--;
		}
	}
	else if (pOther->GetType() == ENT_SPECIAL_BOX)
	{
		//did we collide from the top or bottom?
		if (overlap.ComputeWidth() > overlap.ComputeHeight())
		{
			//hit box with head
			if (overlap.top == marioRect.top)
			{
				SGD::Event* boxHitEvent = new SGD::Event{ "BOX_HIT", nullptr, this };
				SGD::EventManager::GetInstance()->QueueEvent(boxHitEvent, pOther);
			}
		}
	}
	else if (pOther->GetType() == ENT_MUSHROOM || pOther->GetType() == ENT_FLOWER)
	{
		SGD::Event* collectMushroomEvent = new SGD::Event{ "MARIO_COLLECT", nullptr, this };
		SGD::EventManager::GetInstance()->QueueEvent(collectMushroomEvent, pOther);
		m_damageLeft++;

		SGD::AudioManager::GetInstance()->PlayAudio(m_powerUpSfx);

		if (m_damageLeft > 3)
		{
			m_damageLeft = 3;
		}
		//change images to big mario
		if (m_damageLeft == 2)
		{ 
			SetSize({ GetSize().width * 1.25f, GetSize().height * 2 });
		}
	}
}

/*virtual*/ void Mario::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "SCORE")
	{
		int* points = reinterpret_cast<int*>(pEvent->GetData());

		if (points != nullptr)
			SetScore(GetScore() + *points);
	}
}

void Mario::Jump()
{
	//only jump if we aren't already jumping
	if (!m_isJumping)
	{
		//determine which jump sfx to play
		if (m_damageLeft == 2)
			//TODO - THIS NEEDS TO BE CHANGED
			SGD::AudioManager::GetInstance()->PlayAudio(m_smallJumpSfx);
		else
			SGD::AudioManager::GetInstance()->PlayAudio(m_smallJumpSfx);

		SetVelocity({ GetVelocity().x, -1000.0f });
		m_isJumping = true;
	}
}
