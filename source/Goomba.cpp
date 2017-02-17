/***************************************************************
|	File:		Goomba.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "Goomba.h"

#include "CellAnimation.h"
#include "GameplayState.h"
#include "Game.h"
#include "Map.h"
#include "DestroyEntityMessage.h"
#include "Mario.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

#define GRAVITY 2500.0f

Goomba::Goomba() : SGD::Listener(this)
{
	if (m_goombaAnimation == nullptr)
	{
		m_goombaAnimation = new CellAnimation;
		m_goombaAnimation->Initialize(L"resource/graphics/EHT_GOOMBA_WALKING.png", 1, 2, 16, 16, 2, 0.05f);
	}

	SGD::Listener::RegisterForEvent("ENEMY_STOMPED");
	SGD::Listener::RegisterForEvent("ENEMY_HIT");

}

/*virtual*/ Goomba::~Goomba()
{
	if (m_goombaAnimation != nullptr)
	{
		m_goombaAnimation->Terminate();
		delete m_goombaAnimation;
		m_goombaAnimation = nullptr;
	}

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_deadGoombaImg);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_stompSfx);
}

/*virtual*/ void Goomba::Update(float elapsedTime) /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();
	Entity* mario = GameplayState::GetInstance()->GetMario();
	float screenWidth = Game::GetInstance()->GetScreenWidth();

	//if mario is dead then return, the goomba doesn't move
	if (dynamic_cast<Mario*>(mario)->GetDamageLeft() == 0)
		return;

	switch (m_goombaState)
	{
	case GOOMBA_ALIVE:

		//start moving the Goomba once mario is near him
		if ((GetPosition().x - mario->GetPosition().x - map->GetPosition().x) < screenWidth || m_marioNear == true)
		{
			m_marioNear = true;

			//update its animation
			if (m_goombaAnimation != nullptr && m_goombaAnimation->IsPlaying() == true)
				m_goombaAnimation->Update(elapsedTime);

			//only move if the space is OK to move onto, bool keeps track of which way the goomba is moving
			//left movement
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - (128 * elapsedTime) - map->GetPosition().x, GetPosition().y) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - (128 * elapsedTime) - map->GetPosition().x, GetPosition().y + GetSize().height) == true &&
				m_movingLeft == true)
			{
				SetPosition({ GetPosition().x - (128 * elapsedTime), GetPosition().y });
			}
			else
			{
				m_movingLeft = false;
				m_movingRight = true;
			}

			//right movement
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width + (128 * elapsedTime) - map->GetPosition().x, GetPosition().y) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width + (128 * elapsedTime) - map->GetPosition().x, GetPosition().y + GetSize().height) == true &&
				m_movingRight == true)
			{
				SetPosition({ GetPosition().x + (128 * elapsedTime), GetPosition().y });
			}
			else
			{
				m_movingRight = false;
				m_movingLeft = true;
			}

			Entity::Update(elapsedTime);

			//this keeps the goombas from falling through objects they're standing on
			while ((dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - map->GetPosition().x, GetPosition().y + GetSize().height) == false ||
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width - map->GetPosition().x, GetPosition().y + GetSize().height) == false))
				SetPosition({ GetPosition().x, GetPosition().y - 0.1f });

			//this makes the goombas fall when it steps off an object it's standing on
			if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - map->GetPosition().x, GetPosition().y + GetSize().height) == true &&
				dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width - map->GetPosition().x, GetPosition().y + GetSize().height) == true)
				SetVelocity({ GetVelocity().x, 1000.0f });
		}

		break;
	case GOOMBA_DEAD:

		if (m_deathTimer < 0)
		{
			DestroyEntityMessage* msg = new DestroyEntityMessage{ this };

			msg->QueueMessage();
			msg = nullptr;
		}
			//break;

		//determines how long to display the dead goomba
		m_deathTimer -= elapsedTime;
		break;
	case GOOMBA_HIT:
		SetVelocity({ GetVelocity().x, GetVelocity().y + GRAVITY * elapsedTime });
		Entity::Update(elapsedTime);
		break;

	}
}

/*virtual*/ void Goomba::Render(void) /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	switch (m_goombaState)
	{
	case GOOMBA_ALIVE:
		if (m_goombaAnimation->IsPlaying() == false)
			m_goombaAnimation->Restart(true, 0.5f);

		if (m_goombaAnimation != nullptr && m_goombaAnimation->IsPlaying() == true)
			m_goombaAnimation->Render({ GetPosition().x - map->GetPosition().x, GetPosition().y }, 3.0f, { 255, 255, 255 });

		break;
	case GOOMBA_DEAD:
		if (m_deathTimer > 0)
		{
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage,
			{ GetPosition().x - map->GetPosition().x, GetPosition().y },
			{}, {}, {},
			{ 3.0f, 3.0f });
		}
		break;
	default:
		break;
	}
}

/*virtual*/ SGD::Rectangle Goomba::GetRect(void) const /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	return SGD::Rectangle(SGD::Point{ GetPosition().x - map->GetPosition().x, GetPosition().y }, GetSize());
}

/*virtual*/ void Goomba::HandleCollision(const IEntity* pOther) /*override*/
{

}

/*virtual*/ void Goomba::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "ENEMY_STOMPED")
	{
		SetImage(m_deadGoombaImg);
		m_goombaState = GOOMBA_DEAD;
		SGD::AudioManager::GetInstance()->PlayAudio(m_stompSfx);
		SetSize({ 0, 0 });

		//increase mario's score
		int points = 100;

		SGD::Event stompevent = { "SCORE", &points, this };
		stompevent.SendEventNow();
	}
	else if (pEvent->GetEventID() == "ENEMY_HIT")
	{
		SetRotation(SGD::PI / 2);

		SetVelocity({ GetVelocity().x, -1000.0f });

		m_goombaState = GOOMBA_HIT;

		//destroy the entity
		DestroyEntityMessage* msg = new DestroyEntityMessage{ this };

		msg->QueueMessage();
		msg = nullptr;

		//increase mario's score
		int points = 100;

		SGD::Event stompevent = { "SCORE", &points, this };
		stompevent.SendEventNow();
	}
}

