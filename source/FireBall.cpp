#include "FireBall.h"

#include "GameplayState.h"
#include "Map.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

#define GRAVITY 2500.0f

FireBall::FireBall() : SGD::Listener(this)
{

}


FireBall::~FireBall()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_bumpSfx);
}

/*virtual*/ void FireBall::Update(float elapsedTime) /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	//always pulling the fireball back to the ground
	SetVelocity({ GetVelocity().x, GetVelocity().y + GRAVITY * elapsedTime });

	if (dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - (128 * elapsedTime) - map->GetPosition().x, GetPosition().y) == true &&
		dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - (128 * elapsedTime) - map->GetPosition().x, GetPosition().y + GetSize().height) == true)
	{
		//the fireball ran into a solid object, delete it
		//delete this;
		//SetPosition({ GetPosition().x - (128 * elapsedTime), GetPosition().y });
	}

	//if the firball hit the ground, change it's verticle verlocity
	if ((dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x - map->GetPosition().x, GetPosition().y + GetSize().height) == false ||
		dynamic_cast<Map*>(map)->MapMoveOK(GetPosition().x + GetSize().width - map->GetPosition().x, GetPosition().y + GetSize().height) == false))
	{
		SetVelocity({ GetVelocity().x, -500 });
	}

	Entity::Update(elapsedTime);
}

/*virtual*/ void FireBall::Render(void) /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(GetRect(), { 0, 255, 0 });

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage,
	{ GetPosition().x, GetPosition().y },
	GetRotation(),
	{}, {},
	{ 3.0f, 3.0f });
}

/*virtual*/ SGD::Rectangle FireBall::GetRect(void) const /*override*/
{
	Entity* map = GameplayState::GetInstance()->GetMap();

	return SGD::Rectangle(SGD::Point{ GetPosition().x, GetPosition().y }, GetSize());
}

/*virtual*/ void FireBall::HandleCollision(const IEntity* pOther)	/*override*/
{
	if (pOther->GetType() == ENT_GOOMBA)
	{
		SGD::Event* enemyHit = new SGD::Event{ "ENEMY_HIT", nullptr, this };
		SGD::EventManager::GetInstance()->QueueEvent(enemyHit, pOther);

		SGD::AudioManager::GetInstance()->PlayAudio(m_bumpSfx);

		//destroy the entity
		DestroyEntityMessage* msg = new DestroyEntityMessage{ this };

		msg->QueueMessage();
		msg = nullptr;
	}
}

/*virtual*/ void FireBall::HandleEvent(const SGD::Event* pEvent) /*override*/
{

}
