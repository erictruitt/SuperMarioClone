/***************************************************************
|	File:		InstructionsState.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

class InstructionsState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static InstructionsState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entities
	virtual void	Render(float elapsedTime)	override;	// render game entities / menus

private:
	InstructionsState(void) = default;
	virtual ~InstructionsState(void) = default;

	InstructionsState(const InstructionsState&) = delete;	// copy constructor
	InstructionsState& operator= (const InstructionsState&) = delete;	// assignment operator

	SGD::HTexture m_mario = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_MARIO_STANDING.png", { 255, 255, 255 });
	SGD::HTexture m_goomba = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_GOOMBA_WALKING.png", { 255, 255, 255 });
};

