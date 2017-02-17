/***************************************************************
|	File:		SpecialMysteryBox.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "SpecialMysteryBox.h"

#include "GameplayState.h"
#include "CellAnimation.h"
#include "Mario.h"

#include "../SGD Wrappers/SGD_Event.h"

SpecialMysteryBox::SpecialMysteryBox() : SGD::Listener(this)
{
	if (m_boxAnimation == nullptr)
	{
		m_boxAnimation = new CellAnimation;
		m_boxAnimation->Initialize(L"resource/graphics/EHT_MYSTERY_BOX_ANIMATION.png", 1, 3, 16, 16, 3, 0.1f);
	}

	SGD::Listener::RegisterForEvent("BOX_HIT");
}


SpecialMysteryBox::~SpecialMysteryBox()
{
	if (m_boxAnimation != nullptr)
	{
		m_boxAnimation->Terminate();
		delete m_boxAnimation;
		m_boxAnimation = nullptr;
	}

	SGD::AudioManager::GetInstance()->UnloadAudio(m_stompSfx);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_specialBoxImg);
}

/*virtual*/ void SpecialMysteryBox::Update(float elapsedTime) /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	switch (m_boxState)
	{
	case BOX_ACTIVE:
		//update its animation
		if (m_boxAnimation != nullptr && m_boxAnimation->IsPlaying() == true)
			m_boxAnimation->Update(elapsedTime);
		break;
		/*case BOX_INACTIVE:
		if (m_boxPosCorrect > 0)
		{
			m_boxPosCorrect -= 1;

			SetPosition({ GetPosition().x, GetPosition().y + m_boxPosCorrect });

		}
		break;*/
	}
}

/*virtual*/ void SpecialMysteryBox::Render(void) /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	switch (m_boxState)
	{
	case BOX_ACTIVE:

		if (m_boxAnimation->IsPlaying() == false)
			m_boxAnimation->Restart(true, 0.5f);

		if (m_boxAnimation != nullptr && m_boxAnimation->IsPlaying() == true)
			m_boxAnimation->Render({ GetPosition().x - map->GetPosition().x, GetPosition().y }, 3.0f, { 255, 255, 255 });

		break;
	case BOX_INACTIVE:

		SGD::GraphicsManager::GetInstance()->DrawTexture(m_specialBoxImg,
		{ GetPosition().x - map->GetPosition().x, GetPosition().y },
		{}, {}, {},
		{ 3.0f, 3.0f });

		break;
	}
}

/*virtual*/ SGD::Rectangle SpecialMysteryBox::GetRect(void) const /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	return SGD::Rectangle(SGD::Point{ GetPosition().x - map->GetPosition().x, GetPosition().y }, GetSize());
}

/*virtual*/ void SpecialMysteryBox::HandleCollision(const IEntity* pOther)	/*override*/
{

}

/*virtual*/ void SpecialMysteryBox::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	Entity* mario = GameplayState::GetInstance()->GetMario();

	if (pEvent->GetEventID() == "BOX_HIT" && m_boxState == BOX_ACTIVE)
	{
		//create a power up
		GameplayState::GetInstance()->CreatePowerUp(dynamic_cast<Mario*>(mario)->GetDamageLeft(), GetPosition().x, GetPosition().y);

		SetPosition({ GetPosition().x, GetPosition().y});
		SGD::AudioManager::GetInstance()->PlayAudio(m_stompSfx);
		SetSize({ 0, 0 });
		m_boxState = BOX_INACTIVE;
	}
}

