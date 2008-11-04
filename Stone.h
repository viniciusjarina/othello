#ifndef __OTHELLO_STONE_H__
#define __OTHELLO_STONE_H__

class StonesSpriteArray;

class Stone
{
	StonesSpriteArray & m_arrImages;
	int m_iCurrentFrame;
	int m_nCountWait;

	enum AnimationState { STOPED, TURNING_BLACK, TURNING_WHITE, WAITING_OTHER_STONE };
	AnimationState m_state;
	AnimationState m_nextState;

public:

	enum StoneColor { BLACK, WHITE };

	Stone(StonesSpriteArray & images);

	void Logic();

	void Draw(Troll::Surface & s,const Troll::Point & ptPos) const;

public:

	bool AnimationDone() const;
	
	void SetColor(StoneColor color,int nCountWait);
	void SetColorNow(StoneColor color);
	StoneColor GetColor() const;
};

#endif // __OTHELLO_STONE_H__