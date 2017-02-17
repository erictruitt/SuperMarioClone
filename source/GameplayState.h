/***************************************************************
|	File:		GameplayState.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	GameplayState class initializes & runs the game logic
***************************************************************/

#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H


#include "IGameState.h"							// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message



/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Entity;
class EntityManager;

/**************************************************************/
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
	enum ENTITYBUCKET {BUCKET_MAP, BUCKET_ENTITIES};
public:
	/**********************************************************/
	// Singleton Accessor:
	static GameplayState* GetInstance( void );

	Entity* GetMario(void) const { return m_localMario; }
	Entity* GetMap(void) const { return m_pMap; }
	SGD::HAudio GetBackgroundMusic(void) const { return m_backgroundMus; }

	float GetCameraWidth(void) const { return m_cameraWidth; }
	float GetCameraHeight(void) const { return m_cameraHeight; }

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources
	virtual void	Exit	( void )				override;	// unload resources
													
	virtual bool	Update	( float elapsedTime )	override;	// handle input & update entities
	virtual void	Render	( float elapsedTime )	override;	// render game entities / menus

	//void LoadMap(void);

	Entity* CreateMap(void);
	Entity* CreateMario(void);
	void CreateGoomba(int _x, int _y);
	void CreateSpecialBox(int _x, int _y);
	void CreatePowerUp(int _damageLeft, float _x, float _y);
	void CreateFireBall(float _x, float _y);

private:
	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	GameplayState( void )			= default;	// default constructor
	virtual ~GameplayState( void )	= default;	// destructor

	GameplayState( const GameplayState& )				= delete;	// copy constructor
	GameplayState& operator= ( const GameplayState& )	= delete;	// assignment operator

	
	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities			= nullptr;

	Entity* m_pMap = nullptr;
	Entity* m_localMario = nullptr;
	
	/**********************************************************/	
	// Message Callback Function:
	static void MessageProc( const SGD::Message* pMsg );

	SGD::HTexture m_mapImg = SGD::INVALID_HANDLE;
	SGD::HTexture m_marioImg = SGD::INVALID_HANDLE;
	SGD::HAudio m_backgroundMus = SGD::INVALID_HANDLE;
	SGD::HTexture m_mushroomImg = SGD::INVALID_HANDLE;
	SGD::HTexture m_flowerImg = SGD::INVALID_HANDLE;
	SGD::HTexture m_fireballImg = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Pause Menu variables
	int		m_nCursor = 0;
	bool m_gamePaused = false;
	bool m_justpaused = false;

	float m_cameraWidth;
	float m_cameraHeight;
};

#endif //GAMEPLAYSTATE_H
