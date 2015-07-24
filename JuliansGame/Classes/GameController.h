#ifndef _GAMECONTROLLER_HPP_
#define _GAMECONTROLLER_HPP_

#include "cocos2d.h";
#include "Bullet.h";
#include "EnemyCB.h";

USING_NS_CC;

class GameController
{
public:

	GameController();
	~GameController();

	static Vector<Bullet*> bullets;
	static Vector<EnemyCB*> enemies;

	bool init();

	static Bullet* spawnBullet(int type, Vec2 pos, Vec2 tar);

	static EnemyCB* spawnEnemy(int type);

	static void erase( Node* node );

};



#endif // _GAMECONTROLLER_HPP_
