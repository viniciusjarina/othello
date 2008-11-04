
#include "Stone.h"

class Stones
{
	Stone * m_stones[64];

public:
	Stones();
	~Stones();

	bool InitStones(StonesSpriteArray & arrSprites);

	void Logic();

	bool AnimationDone();

	Stone & GetStone(int x,int y);	
};