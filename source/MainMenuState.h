/***************************************************************
|	File:		MainMenuState.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H


#include "IGameState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


/**************************************************************/
// MainMenuState class
//	- handles the main menu
//	- SINGLETON (statically allocated, not dynamic)
class MainMenuState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor
	static MainMenuState* GetInstance( void );

	
	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources
	virtual void	Exit	( void )				override;	// unload resources

	virtual bool	Update	( float elapsedTime )	override;	// handle input & update entites
	virtual void	Render	( float elapsedTime )	override;	// render entities / menu


private:
	/**********************************************************/
	// SINGLETON!
	MainMenuState( void )			= default;
	virtual ~MainMenuState( void )	= default;

	MainMenuState( const MainMenuState& )				= delete;	
	MainMenuState& operator= ( const MainMenuState& )	= delete;


	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;

	int m_musVol;
	int m_sfxVol;
};

#endif //MAINMENUSTATE_H
