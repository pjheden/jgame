#ifndef _GAMELAYER_HPP_
#define _GAMELAYER_HPP_

#include "cocos2d.h"
#include "Player.h"
#include "EnemyCB.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "ryCast.h"

USING_NS_CC;
class Bullet;
class ryCast;

class GameLayer : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	static Scene* _gameScene;

	cocos2d::PhysicsWorld *sceneWorld;
	void setPhyschisWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; }

	GameLayer();
	~GameLayer();

	virtual bool init();
	CREATE_FUNC(GameLayer);

	void update(float dt);

	static bool _dead;

	void bulletControl();

	void removeBullet( Bullet *b ) ;

	void updateScore( int nr );

	bool onContactBegin ( cocos2d::PhysicsContact &contact );

	void lostLayer();

	void initGame();

	void updateArrows();

	void checkHighscore();

	void playerDead();

	void spawnEnemies();

	void retryGame( cocos2d::Ref* pSender );
	void backToMenu( cocos2d::Ref* pSender );

	void doneButton(Ref* sender);


private:
	MenuItemSprite* donebutton_item;
	Layer* _lostLayer;
	Layer* _highscoreLayer;
	cocos2d::ui::TextField* nameField;

	void removeCast ( DrawNode* node );
	void rCast( Vec2 start, Vec2 end );
};

#endif // _GAMELAYER_HPP_