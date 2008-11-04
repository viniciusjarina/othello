#ifndef __SCENE_H__
#define __SCENE_H__


class Scene
{
public:
	virtual bool Logic() = 0;
	virtual void Draw() = 0;
};


#endif //__SCENE_H__