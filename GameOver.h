
#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();


	CREATE_FUNC(GameOver);
};
#endif // __HELLOWORLD_SCENE_H__
