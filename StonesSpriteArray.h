#ifndef __STONES_SPRITE_ARRAY_H__
#define __STONES_SPRITE_ARRAY_H__


class StonesSpriteArray
{
public:
	StonesSpriteArray();
	~StonesSpriteArray();

private:
	StonesSpriteArray(const StonesSpriteArray& other)
	{ // private to avoid copy object with copy ctor
	}
	
	StonesSpriteArray& operator = (const StonesSpriteArray& other)
	{ // private to avoid assign object with operator =
	}

	Troll::Image m_arrImages[60];// Array with animation frame images

public:

	bool LoadSprites();

	const Troll::Surface & GetSprite(int nIdx) const;

};

#endif // __STONES_SPRITE_ARRAY_H__