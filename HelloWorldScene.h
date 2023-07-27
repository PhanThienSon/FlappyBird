

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void CreatePipe(float);
	void update(float);
	bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	bool OnContactBegan(cocos2d::PhysicsContact &contact);

	void stopFly(float);


	CREATE_FUNC(HelloWorld);
private:
	cocos2d::PhysicsWorld *world;
	cocos2d::Sprite *bird;
	cocos2d::Label *label;
	int score = 0;

	bool isFalling = true;

	void setPhysicsWorld(cocos2d::PhysicsWorld *m_world) {
		world = m_world;
	}
};

#endif // __HELLOWORLD_SCENE_H__
