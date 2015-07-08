#ifndef _WORKER_HPP_
#define _WORKER_HPP_

#include "cocos2d.h"

USING_NS_CC;

class Worker : public cocos2d::Sprite
{
public:
	Animate * moveAnimate;

	Worker();
	~Worker();
	static Worker* create();

	void initAnimations();

	void move();

	PhysicsBody* getBody();
};

#endif // _WORKER_HPP_