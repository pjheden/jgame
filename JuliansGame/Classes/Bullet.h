#ifndef _BULLET_HPP_
#define _BULLET_HPP_

#include "cocos2d.h"

USING_NS_CC;

class Bullet : public Node
{
public:

	Bullet();
	~Bullet();
	CREATE_FUNC( Bullet );
	virtual bool init();

	Vec2 getTarget();
	void setTarget( Vec2 target );
	
	Vec2 getSpeed();
	void setSpeed( Vec2 speed );

	int getType();
	void setType( int type );
	
private:
	Vec2 target;
	Vec2 speed;
	int type;
	
};

class Arrow : public Bullet
{
public:
	CREATE_FUNC( Arrow );
	virtual bool init();


private:
	Vec2 target;
	Vec2 speed;
	int type;

};

#endif // _BULLET_HPP_
