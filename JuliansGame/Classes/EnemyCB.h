#ifndef _ENEMYCB_HPP_
#define _ENEMYCB_HPP_

#include "cocos2d.h"

USING_NS_CC;

class EnemyCB : public cocos2d::Sprite
{
public:
	Animate * moveAnimate;
	Animate * idleAnimate;

	EnemyCB();
	~EnemyCB();
	static EnemyCB* create();

	void initAnimations();

	void move();

	void startIdleAnimation();

	PhysicsBody* getBody();

	virtual void shoot( float dt );


private:

};

class Worker : public EnemyCB
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

class Sniper : public EnemyCB
{
public:
	Animate * moveAnimate;
	Animate * idleAnimate;

	Sniper();
	~Sniper();
	static Sniper* create();

	void initAnimations();

	void move();
	void shoot(float dt);
	DrawNode* drawCast( Vec2 start, Vec2 end );
	bool rayCast( Vec2 start, Vec2 end );
	void startIdleAnimation();

	PhysicsBody* getBody();
private:
	bool idle;
};

#endif // _ENEMYCB_HPP_
