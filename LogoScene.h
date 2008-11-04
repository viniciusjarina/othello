#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "Scene.h"

class LogoScene: public Scene
{
public:
	bool Logic();
	void Draw();
};

#define //__LOGO_SCENE_H__