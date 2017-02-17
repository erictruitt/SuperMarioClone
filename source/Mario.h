/***************************************************************
|	File:		Mario.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
***************************************************************/

#pragma once

#include "Entity.h"
#include "Game.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Listener.h"

class AnchorPointAnimation;

class Mario : public Entity, public SGD::Listener
{

protected:
	virtual ~Mario();
public:
	Mario();

	enum MARIOSTATE { MARIO_ALIVE, MARIO_FIRE_POWER, MARIO_STAR_POWER, MARIO_DEAD, MARIO_WIN, MARIO_LOST };


	/**********************************************************/
	// Interface:
	//	- virtual functions to override
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_MARIO; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override;

	//Accessors
	//float GetSpeed(void) const { return m_speed; }
	//float GetAccelerationRate(void) const { return m_accelerationRate; }
	int GetDamageLeft(void) const { return m_damageLeft; }
	//bool StarPowerOn(void) const { return m_starPower; }
	int GetState(void) const { return m_marioState; }
	float GetTimer(void) const { return m_gameTimer; }
	int GetScore(void) const { return m_score; }
	SGD::HAudio GetSFX(void) const { return m_smallJumpSfx; }
	SGD::HAudio GetWinningMus(void) const { return m_levelCompleteMus; }
	SGD::HAudio GetGameOverMus(void) const { return m_gameOverMus; }


	//Mutators
	//void SetSpeed(float _speed) { m_speed = _speed; }
	//void SetAccelerationRate(float _acceleration) { m_accelerationRate = _acceleration; }
	void SetDamageLeft(int _damage) { m_damageLeft = _damage; }
	void SetState(int _state) { m_marioState = static_cast<MARIOSTATE>(_state); }
	//void SetStarPower(bool _power) { m_starPower = _power; }
	void SetTimer(float _timer) { m_gameTimer = _timer; }
	void SetScore(int _score) { m_score = _score; }

	void Jump(void);

private:
	/**********************************************************/
	// Movement
	//float m_speed = 0; //1 == moving
	//float m_accelerationRate = 100;

	bool m_isJumping = false;
	//bool m_lookLeft = false;

	SGD::HTexture m_jumpingImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_MARIO_JUMPING.png");

	SGD::HTexture m_BigJumpingImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_BIG_MARIO_JUMPING.png");
	SGD::HTexture m_BigMarioImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_BIG_MARIO_STANDING.png");

	SGD::HTexture m_FireJumpingImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_FIRE_MARIO_JUMPING.png");
	SGD::HTexture m_FireMarioImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_FIRE_MARIO_STANDING.png");


	/**********************************************************/
	//  Action/Power Variables
	float m_starTimer = 10.0f;
	bool m_starPower = false;
	AnchorPointAnimation* m_walkingAnim = nullptr;
	AnchorPointAnimation* m_BigWalkingAnim = nullptr;
	AnchorPointAnimation* m_FireWalkingAnim = nullptr;

	/**********************************************************/
	// Collision
	int m_damageLeft = 1; //2 == Big Mario / Flower-Power

	/**********************************************************/
	// hud variables
	float m_gameTimer = 90.0f;
	int m_score = 0;
	//int m_coins = 0;

	/**********************************************************/
	// Mario State
	MARIOSTATE m_marioState = MARIO_ALIVE;

	SGD::HTexture m_deadImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/EHT_MARIO_DEAD.png");
	float m_deathTimer = 0.5f;

	/**********************************************************/
	// SFX & Music
	SGD::HAudio m_gameOverMus = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_GAMEOVER.xwm");
	SGD::HAudio m_loseLifeMus = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_LOSE_LIFE_SFX.xwm");
	SGD::HAudio m_smallJumpSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_jump-small.wav");
	SGD::HAudio m_levelCompleteMus = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_LEVEL_COMPLETE.xwm");
	SGD::HAudio m_powerUpSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_powerup.wav");
	SGD::HAudio m_fireBallSfx = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/EHT_FIREBALL.wav");

};

