#include <troll.h>

#include "Stone.h"
#include "StonesSpriteArray.h"

using Troll::Point;

using Troll::Surface;

Stone::Stone( StonesSpriteArray & images ):
m_arrImages(images),
m_iCurrentFrame(0),
m_nCountWait(0),
m_state(STOPED),
m_nextState(STOPED)
{
	
}

void Stone::Logic()
{
	if(m_state == WAITING_OTHER_STONE)
	{
		m_nCountWait--;
		if(m_nCountWait == 0)
		{
			m_state = m_nextState;
			m_nextState = STOPED;
		}
		return;
	}
	if(m_state == STOPED)
		return;

	if(m_state == TURNING_BLACK)
	{
		if(m_iCurrentFrame == 59)
			m_state = STOPED;
		else
			m_iCurrentFrame++;		
	}

	if(m_state == TURNING_WHITE)
	{
		if(m_iCurrentFrame == 0)
			m_state = STOPED;
		else
			m_iCurrentFrame--;		
	}
}

void Stone::Draw(Surface & s,const Point & ptPos) const
{
	const Surface& sprite = m_arrImages.GetSprite(m_iCurrentFrame);

	s.Draw(sprite,ptPos);
}

void Stone::SetColor(Stone::StoneColor color,int nCount)
{
	if(nCount == 0)
	{
		if(color == BLACK && m_iCurrentFrame < 59)
			m_state = TURNING_BLACK;
	    else if(color == WHITE && m_iCurrentFrame > 0)
			m_state = TURNING_WHITE;
	}
	else
	{
		m_state = WAITING_OTHER_STONE;
		m_nCountWait = nCount;

		if(color == BLACK && m_iCurrentFrame < 59)
			m_nextState = TURNING_BLACK;
		else if(color == WHITE && m_iCurrentFrame > 0)
			m_nextState = TURNING_WHITE;
	}
}

bool Stone::AnimationDone() const
{
	return m_state == STOPED;
}

Stone::StoneColor Stone::GetColor() const
{
	if(m_iCurrentFrame == 59)
		return BLACK;
	return WHITE;
}

void Stone::SetColorNow( StoneColor color )
{
	m_nextState = STOPED;
	m_state = STOPED;
	m_nCountWait = 0;

	if(color == BLACK)
	{
		m_iCurrentFrame = 0;
	}
	else
	{
		m_iCurrentFrame = 59;
	}
}