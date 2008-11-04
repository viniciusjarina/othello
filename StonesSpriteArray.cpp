
#include <stdio.h>

#include <troll.h>

#include "StonesSpriteArray.h"

using Troll::Surface;

StonesSpriteArray::StonesSpriteArray()
{
}

StonesSpriteArray::~StonesSpriteArray()
{
}

bool StonesSpriteArray::LoadSprites()
{
	for(int i = 0; i < 60; i++)
	{
		char sFileName[50];
		sprintf(sFileName, "images/stones/pedra_otelo.%d.png",i + 1);
		
		if(!m_arrImages[i].LoadImage(sFileName))
			return false;
	}
	return true;
}

const Surface & StonesSpriteArray::GetSprite( int nIdx ) const
{
	return m_arrImages[nIdx].GetSurface();
}