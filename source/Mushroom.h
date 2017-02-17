/***************************************************************
|	File:		Mushroom.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Mushroom : public Entity, SGD::Listener
{
	enum MUSHROOMSTATE {MUSHROOM_ACTIVE, MUSHROOM_INACTIVE};
public:
	Mushroom();
	virtual ~Mushroom();

	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_MUSHROOM; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther)	override;

	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override;

private:
	MUSHROOMSTATE m_mushroomState = MUSHROOM_ACTIVE;

	bool m_movingLeft = false;
	bool m_movingRight = true;
	bool m_justCreated = true;

};

