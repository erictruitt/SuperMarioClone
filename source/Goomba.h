/***************************************************************
|	File:		Goomba.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "GameplayState.h"

class CellAnimation;

class Goomba : public Entity, public SGD::Listener
{
	enum GOOMBASTATE { GOOMBA_ALIVE, GOOMBA_DEAD, GOOMBA_HIT };

public:
	Goomba();
	virtual ~Goomba();

	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_GOOMBA; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override;

protected:
	GOOMBASTATE m_goombaState = GOOMBA_ALIVE;
	CellAnimation* m_goombaAnimation = nullptr;

	SGD::HTexture m_deadGoombaImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_DEAD_GOOMBA_IMG.png");
	SGD::HAudio m_stompSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_STOMP.wav");


	bool m_movingLeft = true;
	bool m_movingRight = false;
	bool m_marioNear = false;

	float m_deathTimer = 1.0f;
};

