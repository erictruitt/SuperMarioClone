/***************************************************************
|	File:		SpecialMysteryBox.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/
#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

class CellAnimation;

class SpecialMysteryBox : public Entity, public SGD::Listener
{
	enum BOXSTATE{BOX_ACTIVE, BOX_INACTIVE};
public:
	SpecialMysteryBox();
	virtual ~SpecialMysteryBox();

	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_SPECIAL_BOX; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther)	override;

	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override;

private:
	BOXSTATE m_boxState = BOX_ACTIVE;

	CellAnimation* m_boxAnimation = nullptr;

	int m_boxPosCorrect = 5;

	SGD::HAudio m_stompSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_STOMP.wav");
	SGD::HTexture m_specialBoxImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_MYSTERY_BOX_INACTIVE.png");

};

