/***************************************************************
|	File:		Flower.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#include "Flower.h"

#include "GameplayState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"


Flower::Flower() : SGD::Listener(this)
{
	SGD::Listener::RegisterForEvent("MARIO_COLLECT");
}


Flower::~Flower()
{
}

/*virtual*/ void	Flower::Update(float elapsedTime)		/*override*/
{

}

/*virtual*/ void	Flower::Render(void)					/*override*/
{
	if (m_flowerState == FLOWER_ACTIVE)
	{
		Entity* map = GameplayState::GetInstance()->GetMap();

		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage,
		{ GetPosition().x - map->GetPosition().x, GetPosition().y },
		{}, {}, {},
		{ 3.0f, 3.0f });
	}
}

/*virtual*/ SGD::Rectangle Flower::GetRect(void)	const			/*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	return SGD::Rectangle(SGD::Point{ GetPosition().x - map->GetPosition().x, GetPosition().y }, GetSize());
}

/*virtual*/ void	Flower::HandleCollision(const IEntity* pOther)	/*override*/
{
	
}

/*virtual*/ void Flower::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "MARIO_COLLECT")
	{
		SetSize({ 0, 0 });
		m_flowerState = FLOWER_INACTIVE;

		//increase mario's score
		int points = 1000;

		SGD::Event stompevent = { "SCORE", &points, this };
		stompevent.SendEventNow();
	}
}
