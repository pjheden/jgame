#include "EnemyCB.h"

using namespace cocos2d;

EnemyCB::EnemyCB(){}


EnemyCB::~EnemyCB()
{
	//CC_SAFE_RELEASE(moveAnimate);
}

EnemyCB* EnemyCB::create()
{
	EnemyCB* eSprite = new EnemyCB();
	
	eSprite->setScaleX( -1 );

    if (eSprite && eSprite->initWithFile( "C:/JuliansGame/JuliansGame/Resources/cowboy21.png" ) )
    {
		eSprite->initAnimations();
		eSprite->move();

        return eSprite;
    }

    CC_SAFE_DELETE(eSprite);
    return NULL;
}

void EnemyCB::initAnimations()
{

}


PhysicsBody* EnemyCB::getBody()
{
	auto physicsBody = PhysicsBody::createBox( this->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );
	physicsBody->setCollisionBitmask( 2 );
	physicsBody->setContactTestBitmask( true );

	return physicsBody;
}


void EnemyCB::move()
{
	// 1
	auto monsterContentSize = this->getContentSize();
	auto selfContentSize = Director::getInstance()->getWinSize();
	int minY = monsterContentSize.height/2;
	int maxY = selfContentSize.height - monsterContentSize.height/2;
	int rangeY = maxY - minY;
	int randomY = ( rand() % rangeY ) + minY;

	this->setPosition( Vec2( selfContentSize.width + monsterContentSize.width/2, randomY ) );
	this->setName( "EnemyCB" );

	// 2
	int minDuration = 6.0;
	int maxDuration = 14.0;
	int rangeDuration = maxDuration - minDuration;
	int randomDuration = ( rand() % rangeDuration ) + minDuration;
 
	// 3
	auto actionMove = MoveTo::create( randomDuration, Vec2( selfContentSize.width - selfContentSize.width / 4, randomY ) );

	this->runAction( actionMove );
	//does actionMove, then calls monsterOutside when it's done. ( add parameters by: monsterOutside, this, param1, param2)
	//this->runAction( Sequence::create( actionMove, CallFunc::create( std::bind( &HelloWorld::monsterOutside, this ) ), nullptr ) );
}

