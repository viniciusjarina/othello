
#include <troll.h>

#include "Stones.h"

bool Stones::InitStones( StonesSpriteArray & arrSprites )
{
	for(int i = 0; i < 64; i++)
	{
		m_stones[i] = new Stone(arrSprites);
	}
	return true;
}

Stones::Stones()
{
	for(int i = 0; i < 64; i++)
	{
		m_stones[i] = 0;
	}
}

Stones::~Stones()
{
	for(int i = 0; i < 64; i++)
	{
		if(m_stones[i])
			delete m_stones[i];
	}	
}

void Stones::Logic()
{
	for(int i = 0; i < 64; i++)
	{
		Stone & stone1 = *m_stones[i];
		stone1.Logic();
	}
}

Stone & Stones::GetStone( int x,int y )
{
	return *m_stones[x + 8*y];
}

bool Stones::AnimationDone()
{
	for(int i = 0; i < 64; i++)
	{
		Stone & stone1 = *m_stones[i];
		if(!stone1.AnimationDone())
			return false;
	}
	return true;
}