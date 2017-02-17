/***************************************************************
|	File:		Map.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#include "Map.h"
#include "Mario.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"

/*virtual*/ void Map::Update(float elapsedTime) /*override*/
{
	Entity* mario = GameplayState::GetInstance()->GetMario();

	//don't move the world if mario is not in the middle of the screen or if he's dead or if we won
	if (mario->GetPosition().x < (Game::GetInstance()->GetScreenWidth() - (mario->GetSize().width * 3)) * 0.5f ||
		dynamic_cast<Mario*>(mario)->GetState() == Mario::MARIOSTATE::MARIO_DEAD ||
		 dynamic_cast<Mario*>(mario)->GetState() == Mario::MARIOSTATE::MARIO_WIN)
		return;
	else if (GetPosition().x + Game::GetInstance()->GetScreenWidth() > WORLDWIDTH * 16 * 3 )
		return;

	//don't let the world camera move further than the world exists
	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Right))
	{
		//don't move the map if mario is moving right and about to run into a solid object
		if (MapMoveOK(mario->GetPosition().x + mario->GetSize().width + (256 * elapsedTime), mario->GetPosition().y) == true &&
			MapMoveOK(mario->GetPosition().x + mario->GetSize().width + (256 * elapsedTime), mario->GetPosition().y + mario->GetSize().height) == true)
		{
			SetPosition(SGD::Point{ GetPosition().x + (256 * elapsedTime), GetPosition().y });

			//send the event to the enemies to update their positions
			//float eventPosition = GetPosition().x;

			//SGD::Event event = { "MOVE_POS", &eventPosition, this };
			//event.SendEventNow();
		}
	}

}

/*virtual*/ void Map::Render(void) /*override*/
{
	for (int y = 0; y < WORLDHEIGHT; y++)
	{
		for (int x = 0; x < WORLDWIDTH; x++)
		{
			SGD::Rectangle tile = {};

			switch (m_worldArray[x][y])
			{

			case TILE_GROUND:
				tile.left = (TILE_GROUND % 64) * 16;
				tile.top = (TILE_GROUND / 64) * 16;

				break;
			case TILE_BLOCK:
				tile.left = (TILE_BLOCK % 64) * 16;
				tile.top = (TILE_BLOCK / 64) * 16;

				break;
			case TILE_MYSTERY_BOX:
				tile.left = (TILE_MYSTERY_BOX % 64) * 16;
				tile.top = (TILE_MYSTERY_BOX / 64) * 16;

				break;
			case TILE_BLANK_BLOCK:
				tile.left = (TILE_BLANK_BLOCK % 64) * 16;
				tile.top = (TILE_BLANK_BLOCK / 64) * 16;

				break;
			case TILE_PIPE_TOPL:
				tile.left = (TILE_PIPE_TOPL % 64) * 16;
				tile.top = (TILE_PIPE_TOPL / 64) * 16;

				break;
			case TILE_PIPE_TOPR:
				tile.left = (TILE_PIPE_TOPR % 64) * 16;
				tile.top = (TILE_PIPE_TOPR / 64) * 16;

				break;
			case TILE_PIPE_BOTTOML:
				tile.left = (TILE_PIPE_BOTTOML % 64) * 16;
				tile.top = (TILE_PIPE_BOTTOML / 64) * 16;

				break;
			case TILE_PIPE_BOTTOMR:
				tile.left = (TILE_PIPE_BOTTOMR % 64) * 16;
				tile.top = (TILE_PIPE_BOTTOMR / 64) * 16;

				break;
			case TILE_HILL_1:
				tile.left = (TILE_HILL_1 % 64) * 16;
				tile.top = (TILE_HILL_1 / 64) * 16;

				break;
			case TILE_HILL_2:
				tile.left = (TILE_HILL_2 % 64) * 16;
				tile.top = (TILE_HILL_2 / 64) * 16;

				break;
			case TILE_HILL_3:
				tile.left = (TILE_HILL_3 % 64) * 16;
				tile.top = (TILE_HILL_3 / 64) * 16;

				break;
			case TILE_HILL_4:
				tile.left = (TILE_HILL_4 % 64) * 16;
				tile.top = (TILE_HILL_4 / 64) * 16;

				break;
			case TILE_HILL_5:
				tile.left = (TILE_HILL_5 % 64) * 16;
				tile.top = (TILE_HILL_5 / 64) * 16;

				break;
			case TILE_HILL_6:
				tile.left = (TILE_HILL_6 % 64) * 16;
				tile.top = (TILE_HILL_6 / 64) * 16;

				break;
			case TILE_CLOUD_1:
				tile.left = (TILE_CLOUD_1 % 64) * 16;
				tile.top = (TILE_CLOUD_1 / 64) * 16;

				break;
			case TILE_CLOUD_2:
				tile.left = (TILE_CLOUD_2 % 64) * 16;
				tile.top = (TILE_CLOUD_2 / 64) * 16;

				break;
			case TILE_CLOUD_3:
				tile.left = (TILE_CLOUD_3 % 64) * 16;
				tile.top = (TILE_CLOUD_3 / 64) * 16;

				break;
			case TILE_CLOUD_4:
				tile.left = (TILE_CLOUD_4 % 64) * 16;
				tile.top = (TILE_CLOUD_4 / 64) * 16;

				break;
			case TILE_CLOUD_5:
				tile.left = (TILE_CLOUD_5 % 64) * 16;
				tile.top = (TILE_CLOUD_5 / 64) * 16;

				break;
			case TILE_CLOUD_6:
				tile.left = (TILE_CLOUD_6 % 64) * 16;
				tile.top = (TILE_CLOUD_6 / 64) * 16;

				break;
			case TILE_BUSH_1:
				tile.left = (TILE_BUSH_1 % 64) * 16;
				tile.top = (TILE_BUSH_1 / 64) * 16;

				break;
			case TILE_BUSH_2:
				tile.left = (TILE_BUSH_2 % 64) * 16;
				tile.top = (TILE_BUSH_2 / 64) * 16;

				break;
			case TILE_BUSH_3:
				tile.left = (TILE_BUSH_3 % 64) * 16;
				tile.top = (TILE_BUSH_3 / 64) * 16;

				break;
			case TILE_FLAG_POLE:
				tile.left = (TILE_FLAG_POLE % 64) * 16;
				tile.top = (TILE_FLAG_POLE / 64) * 16;

				break;
			case TILE_FLAG_TOP:
				tile.left = (TILE_FLAG_TOP % 64) * 16;
				tile.top = (TILE_FLAG_TOP / 64) * 16;

				break;
			case TILE_CASTLE_1:
				tile.left = (TILE_CASTLE_1 % 64) * 16;
				tile.top = (TILE_CASTLE_1 / 64) * 16;

				break;
			case TILE_CASTLE_2:
				tile.left = (TILE_CASTLE_2 % 64) * 16;
				tile.top = (TILE_CASTLE_2 / 64) * 16;

				break;
			case TILE_CASTLE_3:
				tile.left = (TILE_CASTLE_3 % 64) * 16;
				tile.top = (TILE_CASTLE_3 / 64) * 16;

				break;
			case TILE_CASTLE_4:
				tile.left = (TILE_CASTLE_4 % 64) * 16;
				tile.top = (TILE_CASTLE_4 / 64) * 16;

				break;
			case TILE_CASTLE_5:
				tile.left = (TILE_CASTLE_5 % 64) * 16;
				tile.top = (TILE_CASTLE_5 / 64) * 16;

				break;
			case TILE_CASTLE_6:
				tile.left = (TILE_CASTLE_6 % 64) * 16;
				tile.top = (TILE_CASTLE_6 / 64) * 16;

				break;
			case TILE_CASTLE_7:
				tile.left = (TILE_CASTLE_7 % 64) * 16;
				tile.top = (TILE_CASTLE_7 / 64) * 16;

				break;
			default:
				continue;
			}

			tile.right = tile.left + 16;
			tile.bottom = tile.top + 16;

			SGD::GraphicsManager::GetInstance()->DrawTextureSection(GetImage(),
				SGD::Point{ ((x * 16.0f) * 3) - GetPosition().x, y * 16.0f * 3},
				tile, {}, {}, {},
				{ 3.0f, 3.0f });
		}
	}
}

void Map::LoadWorld(void)
{
	fstream fin;
	fin.open("resource/graphics/EHT_MAP.txt", ios_base::in);

	if (fin.is_open())
	{
		int tile;

		for (int y = 0; y < WORLDHEIGHT; y++)
		{
			for (int x = 0; x < WORLDWIDTH; x++)
			{
				fin >> tile;

				tile--;

				m_worldArray[x][y] = tile;

				if (tile == TILE_GOOMBA)
					GameplayState::GetInstance()->CreateGoomba(x, y);
				else if (tile == TILE_SPECIAL_BOX)
				{
					GameplayState::GetInstance()->CreateSpecialBox(x, y);
				}
				
				//TODO - create koopa, mushroom, flower & starman

				fin.ignore(INT_MAX, ',');
			}
		}

		fin.close();
	}
}

//GetMapID
// -returns true if the mapID being accessed is NOT a solid object
// - returns false otherwise
bool Map::MapMoveOK(float _x, float _y)
{
	//scale the variables back to account for the position of the map relative to where mario is on screen
	int mx = (int)(((_x + GetPosition().x) / 3.0f) / 16.0f);
	int my = (int)((_y / 3.0f) / 16.0f);

	int mapID = m_worldArray[mx][my];

	switch (mapID)
	{
	case TILE_GROUND: return false;
	case TILE_BLOCK: return false;
	case TILE_MYSTERY_BOX: return false;
	case TILE_SPECIAL_BOX: return false;
	case TILE_BLANK_BLOCK: return false;
	case TILE_PIPE_TOPL: return false;
	case TILE_PIPE_TOPR: return false;
	case TILE_PIPE_BOTTOML: return false;
	case TILE_PIPE_BOTTOMR: return false;
	}

	return true;
}

//checks for the win condition
//returns true if mario is at the flag pole, otherwise returns false
bool Map::ReturnWin(float _x, float _y)
{
	//scale the variables back to account for the position of the map relative to where mario is on screen
	int mx = (int)(((_x + GetPosition().x) / 3.0f) / 16.0f);
	int my = (int)((_y / 3.0f) / 16.0f);

	int mapID = m_worldArray[mx][my];

	if (mapID == TILE_FLAG_POLE)
		return true;

	return false;
}

//used during the MARIO_WIN state, used to signify that mario has reached the castle door and we should stop rendering him
//returns true if mario is at the castle door, otherwise returns false
bool Map::CastleDoor(float _x, float _y)
{
	//scale the variables back to account for the position of the map relative to where mario is on screen
	int mx = (int)(((_x + GetPosition().x) / 3.0f) / 16.0f);
	int my = (int)((_y / 3.0f) / 16.0f);

	int mapID = m_worldArray[mx][my];

	if (mapID == TILE_CASTLE_6 || mapID == TILE_CASTLE_7)
		return true;

	return false;
}