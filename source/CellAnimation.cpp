/***************************************************************
|	File:		CellAnimation.cpp
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	CellAnimation class runs a fixed-size animation
|				from frame 0 -> n-1
***************************************************************/

#include "CellAnimation.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>


/**************************************************************/
// Initialize
//	- hardcode the animation for "SGD_Anim_Explosion.png"
//void CellAnimation::Initialize( void )
void CellAnimation::Initialize(const wchar_t* _filename, int _rows, int _cols, int _frameWidth, int _frameHeight, int _numFrames, float _duration)
{
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		_filename);

	m_nNumRows = _rows;
	m_nNumCols = _cols;
	m_nFrameWidth = _frameWidth;
	m_nFrameHeight = _frameHeight;

	m_nCurrFrame = 0;
	m_nNumFrames = _numFrames;

	m_fTimeWaited = 0.0f;
	m_fDuration = _duration;
	m_fSpeed = 1.0f;

	m_bPlaying = false;
	m_bLooping = false;
	m_bFinished = false;
}

/**************************************************************/
// Terminate
//	- unload the resources
void CellAnimation::Terminate(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}


/**************************************************************/
// Update
//	- run the animation timer
void CellAnimation::Update(float elapsedTime)
{
	// Is the animation paused?
	if (m_bPlaying == false)
		return;


	// Increase the timer
	m_fTimeWaited += elapsedTime * m_fSpeed;

	// Is it time to move to the next frame?
	if (m_fTimeWaited >= m_fDuration)
	{
		m_fTimeWaited = 0.0f;
		++m_nCurrFrame;


		// Has it reached the end?
		if (m_nCurrFrame == m_nNumFrames)
		{
			// Should the animation loop from the beginning?
			if (m_bLooping == true)
				m_nCurrFrame = 0;
			else
			{
				// Stop on the last valid frame
				--m_nCurrFrame;
				m_bPlaying = false;
				m_bFinished = true;
			}
		}
	}
}


/**************************************************************/
// Render
//	- draw the current frame at the given position
void CellAnimation::Render(SGD::Point position, float scale, SGD::Color color) const
{
	// Validate the image
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "CellAnimation::Render - image was not initialized!");


	// Is the color invisible? Or is the scale invisible?
	if (color.alpha == 0 || scale == 0.0f)
		return;


	// Calculate the source rect for the current frame
	SGD::Rectangle frame;
	frame.left = float((m_nCurrFrame % m_nNumCols) * m_nFrameWidth);
	frame.top = float((m_nCurrFrame / m_nNumCols) * m_nFrameHeight);
	frame.right = frame.left + m_nFrameWidth;
	frame.bottom = frame.top + m_nFrameHeight;


	// Draw
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage, position, frame,
		0.0f, {},
		color, { scale, scale });
}


/**************************************************************/
// Restart
//	- start the animation over from frame 0
void CellAnimation::Restart(bool looping, float speed)
{
	// Store the parameters
	m_bLooping = looping;
	m_fSpeed = speed;

	// Reset animation
	m_nCurrFrame = 0;
	m_fTimeWaited = 0.0f;
	m_bPlaying = true;
	m_bFinished = false;
}
