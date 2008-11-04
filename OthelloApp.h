#ifndef __OTHELLO_APP_H__
#define __OTHELLO_APP_H__

#include "StonesSpriteArray.h"

class OthelloApp
{
public:
	OthelloApp();

public:
	int Run();

protected:

	bool Init();
	void Cleanup();
};


#endif // __OTHELLO_APP_H__