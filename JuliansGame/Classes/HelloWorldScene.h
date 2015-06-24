#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"

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

		// Physics Contact Listener.....
	bool onContactBegin ( cocos2d::PhysicsContact &contact );

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	//sprite
	cocos2d::Sprite *mySprite;

	//removes sprite when it's called
	void actionFinished(cocos2d::Sprite *bullet);

	//creates a enemy seal-sprite
	void createEnemyseal(cocos2d::Size visibleSize);

	Player* pSprite;

	void update(float dt);

	void menuCloseCallback( cocos2d::Ref* pSender );
	void startGame( cocos2d::Ref* pSender );
	void settings( cocos2d::Ref* pSender );

	void mainMenu();
};

#endif // __HELLOWORLD_SCENE_H__
