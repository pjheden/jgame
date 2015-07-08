#include "Worker.h"

using namespace cocos2d;

Worker::Worker(){}


Worker::~Worker()
{
	//CC_SAFE_RELEASE(moveAnimate);
}

Worker* Worker::create()
{
	Worker* eSprite = new Worker();

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
	//	// load and cache the texture and sprite frames
	//auto cacher = SpriteFrameCache::getInstance();
	//cacher->addSpriteFramesWithFile("C:/JuliansGame/JuliansGame/Resources/indian_walk.plist");

	//#include <sstream>
	//// load all the animation frames into an array
	//const int kNumberOfFrames = 4;
	//Vector<SpriteFrame*> frames;
	//for (int i = 1; i < kNumberOfFrames; i++)
	//{
	//	std::string s = std::to_string( i );

	//	SpriteFrame* aFrame =
	//	cacher->getSpriteFrameByName( "indian_walk" + s + ".png" );
	//	frames.pushBack(aFrame);
	//}

	//// play the animation
	//auto animation = Animation::createWithSpriteFrames(frames, 0.40f);
	//animation->setRestoreOriginalFrame(true);
	//moveAnimate = Animate::create(animation);
	//moveAnimate->retain();
	//this->runAction( RepeatForever::create( idleAnimate ) );
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
	this->setName( "Worker" );

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

	return physicsBody;
}