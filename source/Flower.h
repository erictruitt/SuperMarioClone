/***************************************************************
|	File:		Flower.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#pragma once
#include "Entity.h"

#include"../SGD Wrappers/SGD_Listener.h"

class Flower : public Entity, SGD::Listener
{
	enum FLOWERSTATE { FLOWER_ACTIVE, FLOWER_INACTIVE };

public:
	Flower();
	virtual ~Flower();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_FLOWER; }
	virtual SGD::Rectangle GetRect(void)	const			override;
	virtual void	HandleCollision(const IEntity* pOther)	override;

	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override;

private:
	FLOWERSTATE m_flowerState = FLOWER_ACTIVE;

};

