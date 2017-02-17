/***************************************************************
|	File:		AnchorPointAnimation.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	AnchorPointAnimation class runs a variable-size 
|				animation, storing the source rects for each frame
|				and an 'anchor-point' == relative position
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <vector>
#include <cassert>


/**************************************************************/
// AnchorPointAnimation class
//	- runs animation using an image of variable-size frames
//	- stores a relative offset from the top-left corner of the frame rect 
//	  to the render position
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)
class AnchorPointAnimation
{
public:
	/**********************************************************/
	// Default Constructor & Destructor
	AnchorPointAnimation()	= default;
	~AnchorPointAnimation()	= default;
	

	/**********************************************************/
	// Initialize & Terminate
	void	Initialize(const wchar_t* _filename, SGD::Rectangle _rect1, SGD::Rectangle _rect2, SGD::Rectangle _rect3, SGD::Point _pt1, SGD::Point _pt2, SGD::Point _pt3);	// should have a config file
	void	Terminate	( void );


	/**********************************************************/
	// Animation Controls:
	void	Update		( float elapsedTime );
	void	Render		( SGD::Point position, bool flipped,
						  float scale, SGD::Color color ) const;
	
	SGD::Rectangle GetRect( SGD::Point position, bool flipped,
						  float scale ) const;

	void	Restart		( bool looping, float speed );
	void	Pause		( bool pause )	{	m_bPlaying = !pause;	}

	bool	IsPlaying	( void ) const	{	return m_bPlaying;		}
	bool	IsFinished	( void ) const	{	return m_bFinished;		}

private:
	/**********************************************************/
	// image
	SGD::HTexture			m_hImage		= SGD::INVALID_HANDLE;

	// frame data
	struct Frame
	{
		SGD::Rectangle		rFrame;		// source rectangle
		SGD::Point			ptAnchor;	// relative position within source
		float				fDuration;	// time to wait on this frame
	};

	std::vector< Frame >	m_vFrames;

	// animation data
	int						m_nCurrFrame	= 0;

	float					m_fTimeWaited	= 0.0f;
	float					m_fSpeed		= 1.0f;		// multiplier: 2.0 - twice as fast

	bool					m_bPlaying		= false;
	bool					m_bLooping		= false;
	bool					m_bFinished		= false;
};
