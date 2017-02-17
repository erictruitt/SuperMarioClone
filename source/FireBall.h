#pragma once
#include "Entity.h"

#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

class FireBall : public Entity, SGD::Listener
{
public:
	FireBall();
	virtual ~FireBall();

	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_FIREBALL; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther)	override;

	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override;

private:
	SGD::HAudio m_bumpSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_FIREBALL.wav");

};

