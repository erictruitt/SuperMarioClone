/***************************************************************
|	File:		Mushroom.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "Mushroom.h"

#include "GameplayState.h"
#include "Map.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

Mushroom::Mushroom() : SGD::Listener(this)
{
	SGD::Listener::RegisterForEvent("MARIO_COLLECT");
}


Mushroom::~Mushroom()
{

}

/*virtual*/ void Mushroom::Update(float elapsedTime) /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	while (m_justCreated == true)
	{
		if ((dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - map->GetPosition().x, GetPosition().y + GetSize().height) == false ||
			dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width - map->GetPosition().x, GetPosition().y + GetSize().height) == false))
		{
			SetPosition({ GetPosition().x, GetPosition().y - 128 * elapsedTime });
			Entity::Update(elapsedTime);
			return;
		}
		else
			m_justCreated = false;
	}
	

	
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

/*virtual*/ void Mushroom::Render(void) /*override*/
{
	if (m_mushroomState == MUSHROOM_ACTIVE)
	{
		Entity* map = GameplayState::GetInstance()->GetMap();

		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage,
		{ GetPosition().x - map->GetPosition().x, GetPosition().y },
		{}, {}, {},
		{ 3.0f, 3.0f });
	}
}

/*virtual*/ SGD::Rectangle Mushroom::GetRect(void) const /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	return SGD::Rectangle(SGD::Point{ GetPosition().x - map->GetPosition().x, GetPosition().y }, GetSize());
}

/*virtual*/ void Mushroom::HandleCollision(const IEntity* pOther)	/*override*/
{

}

/*virtual*/ void Mushroom::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "MARIO_COLLECT")
	{
		SetSize({ 0, 0 });
		m_mushroomState = MUSHROOM_INACTIVE;

		//increase mario's score
		int points = 1000;

		SGD::Event stompevent = { "SCORE", &points, this };
		stompevent.SendEventNow();
	}
}
