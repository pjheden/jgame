#ifndef _GAMELAYER_HPP_
#define _GAMELAYER_HPP_

#include "cocos2d.h"
#include "Player.h"
#include "EnemyCB.h"

USING_NS_CC;
class Bullet;

class GameLayer : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	cocos2d::PhysicsWorld *sceneWorld;
	void setPhyschisWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; }

	GameLayer();
	~GameLayer();

	virtual bool init();
	CREATE_FUNC(GameLayer);

	void update(float dt);

	Player* _player;
	static bool _dead;

	void bulletControl();

	void removeBullet( Bullet *b ) ;

	void updateScore( int nr );

	bool onContactBegin ( cocos2d::PhysicsContact &contact );
};

#endif // _GAMELAYER_HPP_