#include "EnemyCB.h"
#include "GameController.h"
#include "stdReplacer.h"

using namespace cocos2d;

EnemyCB::EnemyCB(){}


EnemyCB::~EnemyCB()
{
	CC_SAFE_RELEASE(moveAnimate);
}

EnemyCB* EnemyCB::create()
{
	EnemyCB* eSprite = new EnemyCB();
	eSprite->setName( "EnemyCB" );
	
	eSprite->setScaleX( -1 );

    if (eSprite && eSprite->initWithFile( "cowboy21.png" ) )
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
	// load and cache the texture and sprite frames
	auto cacher = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile("cowboy.plist");

	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames = 2;
	Vector<SpriteFrame*> frames;
	for (int i = 1; i < kNumberOfFrames; i++)
	{
		std::string s = stdReplacer::to_string( i );

		SpriteFrame* aFrame =
		cacher->getSpriteFrameByName( "cowboy2" + s + ".png" );
		frames.pushBack(aFrame);
	}

	// play the animation
	auto animation = Animation::createWithSpriteFrames(frames, 0.20f);
	animation->setRestoreOriginalFrame(true);
	moveAnimate = Animate::create(animation);
	moveAnimate->retain();
	this->runAction( RepeatForever::create( moveAnimate ) );
}

PhysicsBody* EnemyCB::getBody()
{
	auto physicsBody = PhysicsBody::createBox( this->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

	//physicsBody->setCategoryBitmask( 2 );
	physicsBody->setCollisionBitmask( 2 );
	physicsBody->setContactTestBitmask( true );

	physicsBody->setRotationEnable ( false );

	return physicsBody;
}


void EnemyCB::move()
{
	// 1
	auto monsterContentSize = this->getContentSize();
	auto selfContentSize = Director::getInstance()->getWinSize();
	int minY = monsterContentSize.height/2;
	int maxY = selfContentSize.height - monsterContentSize.height;
	int rangeY = maxY - minY;
	int randomY = ( rand() % rangeY ) + minY;

	this->setPosition( Vec2( selfContentSize.width + monsterContentSize.width/2, randomY ) );

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


void EnemyCB::shoot( float dt )
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto tar = Vec2( origin.x, this->getPosition().y );

    GameController::spawnBullet( 2, Vec2( this->getPosition().x - this->getContentSize().width, this->getPosition().y), tar );
}


//------------------------------Worker ------------------

Worker::Worker(){}


Worker::~Worker()
{
	CC_SAFE_RELEASE(moveAnimate);
}

Worker* Worker::create()
{
	Worker* eSprite = new Worker();
	eSprite->setName( "Worker" );


    if (eSprite && eSprite->initWithFile( "C:/JuliansGame/JuliansGame/Resources/Constructor21.png" ) )
    {
		eSprite->initAnimations();
		eSprite->move();

        return eSprite;
    }

    CC_SAFE_DELETE(eSprite);
    return NULL;
}

void Worker::initAnimations()
{
	// load and cache the texture and sprite frames
	auto cacher = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile("constructor.plist");

	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames = 4;
	Vector<SpriteFrame*> frames;
	for (int i = 1; i < kNumberOfFrames; i++)
	{
		std::string s = stdReplacer::to_string( i );

		SpriteFrame* aFrame =
		cacher->getSpriteFrameByName( "Constructor2" + s + ".png" );
		frames.pushBack(aFrame);
	}

	// play the animation
	auto animation = Animation::createWithSpriteFrames(frames, 0.20f);
	animation->setRestoreOriginalFrame(true);
	moveAnimate = Animate::create(animation);
	moveAnimate->retain();
	this->runAction( RepeatForever::create( moveAnimate ) );
}


void Worker::move()
{
	// 1
	auto monsterContentSize = this->getContentSize();
	auto selfContentSize = Director::getInstance()->getWinSize();
	int minY = monsterContentSize.height/2;
	int maxY = selfContentSize.height - monsterContentSize.height/2;
	int rangeY = maxY - minY;
	int randomY = ( rand() % rangeY ) + minY;

	this->setPosition( Vec2( selfContentSize.width + monsterContentSize.width/2, randomY ) );

	// 2
	int minDuration = 12.0;
	int maxDuration = 14.0;
	int rangeDuration = maxDuration - minDuration;
	int randomDuration = ( rand() % rangeDuration ) + minDuration;
 
	// 3
	auto actionMove = MoveTo::create( randomDuration, Vec2( -monsterContentSize.width/2, randomY ) );

	this->runAction( actionMove );
	//does actionMove, then calls monsterOutside when it's done. ( add parameters by: monsterOutside, this, param1, param2)
	//this->runAction( Sequence::create( actionMove, CallFunc::create( std::bind( &HelloWorld::monsterOutside, this ) ), nullptr ) );

}

PhysicsBody* Worker::getBody()
{
	auto physicsBody = PhysicsBody::createBox( this->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	physicsBody->setCollisionBitmask( 2 );
	physicsBody->setContactTestBitmask( true );
	physicsBody->setRotationEnable( false );

	return physicsBody;
}
