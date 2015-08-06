#ifndef _GAMECONTROLLER_HPP_
#define _GAMECONTROLLER_HPP_

#include "cocos2d.h"
#include "Bullet.h"
#include "EnemyCB.h"
#include "Player.h"


USING_NS_CC;

class GameController
{
public:

	GameController();
	~GameController();

	static Vector<Bullet*> bullets;
	static Vector<EnemyCB*> enemies;
	static Player* _player;


	bool init();

	static Bullet* spawnBullet(int type, Vec2 pos, Vec2 tar);

	static EnemyCB* spawnEnemy(int type);

	static Player* spawnPlayer();

	static void erase( Node* node );

	static void eraseAll();

};



#endif // _GAMECONTROLLER_HPP_
