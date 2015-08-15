#ifndef _GAMECONTROLLER_HPP_
#define _GAMECONTROLLER_HPP_

#include "cocos2d.h"
#include "Bullet.h"
#include "EnemyCB.h"
#include "Player.h"
#include "ryCast.h"

USING_NS_CC;

class GameController
{
public:

	GameController();
	~GameController();

	static Vector<Bullet*> bullets;
	static Vector<EnemyCB*> enemies;
	static std::vector<ryCast*> GameController::casts;
	static Player* _player;


	bool init();

	static Bullet* spawnBullet(int type, Vec2 pos, Vec2 tar);

	static EnemyCB* spawnEnemy(int type);

	static Player* spawnPlayer();
	
	static void drawCast( Vec2 start, Vec2 end );

	static void erase( Node* node );

	static void eraseAll();

};


#endif // _GAMECONTROLLER_HPP_
