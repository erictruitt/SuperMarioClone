/***************************************************************
|	File:		OptionsState.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"

class OptionsState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static OptionsState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update entities
	virtual void	Render(float elapsedTime)	override;	// render game entities / menus

private:
	OptionsState(void) = default;
	virtual ~OptionsState(void) = default;

	OptionsState(const OptionsState&) = delete;	// copy constructor
	OptionsState& operator= (const OptionsState&) = delete;	// assignment operator

	SGD::HAudio m_background = SGD::INVALID_HANDLE;
	SGD::HAudio m_sfx = SGD::INVALID_HANDLE;

	int	 m_nCursor = 0;

	int m_musVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
	int m_sfxVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
};

