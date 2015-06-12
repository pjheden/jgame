#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	cocos2d::PhysicsWorld *sceneWorld;

	void setPhyschisWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; }

	bool onContactBegin ( cocos2d::PhysicsContact &contact );

	//sprite
	cocos2d::Sprite *mySprite;
	//cocos2d::Sprite *bullet;

	void actionFinished(cocos2d::Sprite *bullet);

	void spriteJump(cocos2d::Sprite *aSprite);
};

#endif // __HELLOWORLD_SCENE_H__
