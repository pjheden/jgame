#include "Bullet.h"
USING_NS_CC;

Bullet::Bullet(void)
{
}
Bullet::~Bullet(void)
{
}

bool Bullet::init()
{
	Sprite* model = Sprite::create( "bullet.png" );
	if( model )
	{
		auto bulletBody = PhysicsBody::createBox( model->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

		bulletBody->setCollisionBitmask( 4 );
		bulletBody->setContactTestBitmask( true );

		model->setPhysicsBody( bulletBody );
		Bullet::setSpeed( Vec2( 120, 0 ) );
		Bullet::setType( 2 ); 
		addChild( model );
		return true;
	}
	return false;

}

void Bullet::setTarget( Vec2 target )
{
	Bullet::target = target;
}
Vec2 Bullet::getTarget()
{
	return Bullet::target;
}

void Bullet::setSpeed( Vec2 speed )
{
	Bullet::speed = speed;
}
Vec2 Bullet::getSpeed()
{
	return Bullet::speed;
}

void Bullet::setType( int type )
{
	Bullet::type = type;
}

int Bullet::getType()
{
	return Bullet::type;
}

	


bool Arrow::init()
{
	Sprite* arrowModel = Sprite::create( "arrow.png" );
	if( arrowModel )
	{

		auto arrowBody = PhysicsBody::createBox( arrowModel->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

		arrowBody->setCollisionBitmask( 3 );
		arrowBody->setContactTestBitmask( true );

		arrowModel->setPhysicsBody( arrowBody );
		//Note, didn't work to use Arrow::setSpeed because when it's used in
		//other classes and declared as a Bullet* it uses Bullets functions
		Bullet::setSpeed( Vec2( 120, 0 ) );
		Bullet::setType( 1 );
		addChild( arrowModel );
		return true;
	}
	return false;
}
