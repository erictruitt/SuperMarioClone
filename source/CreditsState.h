/***************************************************************
|	File:		CreditsState.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#pragma once
#include "IGameState.h"

#include "Game.h"

class CreditsState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static CreditsState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void) override;	// load resources
	virtual void	Exit(void) override {}	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entities
	virtual void	Render(float elapsedTime)	override;	// render game entities / menus

private:
	CreditsState(void) = default;
	virtual ~CreditsState(void) = default;

	CreditsState(const CreditsState&) = delete;	// copy constructor
	CreditsState& operator= (const CreditsState&) = delete;	// assignment operator

	float m_scrollControl;
	float scrollreturn = -1;
};

