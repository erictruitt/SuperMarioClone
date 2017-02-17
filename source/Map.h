/***************************************************************
|	File:		Map.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#pragma once

#include "Game.h"
#include "Entity.h"
#include "EntityManager.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include <fstream>
using namespace std;

#define WORLDWIDTH 212
#define WORLDHEIGHT 14

class Mario;

class Map : public Entity
{
	//World variables
	enum TileSetIndex {
		TILE_GROUND = 0, TILE_BLOCK = 1, TILE_MYSTERY_BOX = 24, TILE_BLANK_BLOCK = 64, TILE_SPECIAL_BOX = 152,
		TILE_PIPE_TOPL = 512, TILE_PIPE_TOPR = 513, TILE_PIPE_BOTTOML = 576, TILE_PIPE_BOTTOMR = 577,
		TILE_HILL_1 = 520, TILE_HILL_2 = 521, TILE_HILL_3 = 522, TILE_HILL_4 = 584, TILE_HILL_5 = 585, TILE_HILL_6 = 586,
		TILE_CLOUD_1 = 1280, TILE_CLOUD_2 = 1281, TILE_CLOUD_3 = 1282, TILE_CLOUD_4 = 1344, TILE_CLOUD_5 = 1345, TILE_CLOUD_6 = 1346,
		TILE_BUSH_1 = 587, TILE_BUSH_2 = 588, TILE_BUSH_3 = 589,
		TILE_FLAG_POLE = 592, TILE_FLAG_TOP = 528,
		TILE_CASTLE_1 = 11, TILE_CASTLE_2 = 12, TILE_CASTLE_3 = 13, TILE_CASTLE_4 = 14, TILE_CASTLE_5 = 75, TILE_CASTLE_6 = 76, TILE_CASTLE_7  = 77,
		TILE_GOOMBA = 1348
	};

public:
	Map(void) = default;
	virtual ~Map(void) = default;

	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	void LoadWorld(void);
	bool MapMoveOK(float _x, float _y);
	bool ReturnWin(float _x, float _y);
	bool CastleDoor(float _x, float _y);

private:
	int m_worldArray[WORLDWIDTH][WORLDHEIGHT];
	int m_xMovement = 0;
};

