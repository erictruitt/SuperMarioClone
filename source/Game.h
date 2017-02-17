/***************************************************************
|	File:		Game.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef GAME_H
#define GAME_H


/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class BitmapFont;
class IGameState;


/**************************************************************/
// Game class
//	- runs the game logic
//	- controls the SGD wrappers
//	- SINGLETON!
//		- there is only ONE instance
//		- global access method (GetInstance)
class Game
{
public:
	/**********************************************************/
	// Singleton Accessors:
	static Game*	GetInstance		( void );
	static void		DeleteInstance	( void );
	
	/**********************************************************/
	// Setup, Play, Cleanup:
	bool Initialize	( float width, float height );
	int	 Update		( void );
	void Terminate	( void );
	
	/**********************************************************/
	// Screen Size Accessors:
	float				GetScreenWidth	( void ) const	{	return m_fScreenWidth;	}
	float				GetScreenHeight ( void ) const	{	return m_fScreenHeight;	} 

	// Font Accessor: (#include "BitmapFont.h" to use!)
	const BitmapFont*	GetFont			( void ) const	{	return m_pFont;			}
	
	/**********************************************************/
	// Game State Machine:
	//	- can ONLY be called by the state's Input, Update, or Render methods!!!
	void ChangeState( IGameState* pNewState );

private:
	/**********************************************************/
	// Singleton Object:
	static Game*	s_pInstance;

	Game( void )	= default;		// default constructor
	~Game( void )	= default;		// destructor

	Game( const Game& )				= delete;	// copy constructor
	Game& operator= ( const Game& )	= delete;	// assignment operator

	/**********************************************************/
	// Screen Size
	float					m_fScreenWidth	= 1;
	float					m_fScreenHeight = 1;
	bool windowed = true;

	/**********************************************************/
	// Game Font
	BitmapFont*				m_pFont			= nullptr;

	/**********************************************************/
	// Current Game State
	IGameState*				m_pCurrState	= nullptr;

	/**********************************************************/
	// Game Time
	unsigned long			m_ulGameTime	= 0;
};

#endif //GAME_H
