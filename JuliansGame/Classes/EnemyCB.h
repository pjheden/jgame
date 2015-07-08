#ifndef _ENEMYCB_HPP_
#define _ENEMYCB_HPP_

#include "cocos2d.h"

USING_NS_CC;

class EnemyCB : public cocos2d::Sprite
{
public:
	Animate * moveAnimate;

	EnemyCB();
	~EnemyCB();
	static EnemyCB* create();

	void initAnimations();

	void move();


	PhysicsBody* getBody();

private:

};

#endif // _ENEMYCB_HPP_
