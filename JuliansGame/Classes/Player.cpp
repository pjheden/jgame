#include "Player.h"

using namespace cocos2d;

Player::Player() {}

Player::~Player() 
{
//if retained, must release
	CC_SAFE_RELEASE(idleAnimate);
	CC_SAFE_RELEASE(moveAnimate);
}

Player* Player::create()
{
    Player* pSprite = new Player();
	auto physicsBody = PhysicsBody::createBox( pSprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	pSprite->setPhysicsBody( physicsBody );

    if (pSprite->initWithFile( "C:/JuliansGame/JuliansGame/Resources/idle_1.png" ) )
    {
        pSprite->autorelease();

        pSprite->initOptions();

        pSprite->addEvents();

		pSprite->initAnimations();

        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void Player::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
}

void Player::addEvents()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {   
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();

        if(rect.containsPoint(p))
        {
            return true; // to indicate that we have consumed it.
        }

        return false; // we did not consume this event, pass thru.
    };

    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
		auto openGl_location = touch-> getLocation();
        Player::touchEvent( touch, openGl_location );
    };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void Player::touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _point)
{
    CCLOG("touched Player");
}

void Player::initAnimations()
{
	// set the appropriate resource directory for this device
	FileUtils::getInstance()->addSearchResolutionsOrder("playersheet/HD");

	// load and cache the texture and sprite frames
	auto cacher = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile("walk.plist");

	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames = 4;
	Vector<SpriteFrame*> frames;
	for (int i = 1; i < kNumberOfFrames; i++)
	{
		std::string s = std::to_string( i );

		SpriteFrame* aFrame =
		cacher->getSpriteFrameByName( "move_" + s + ".png" );
		frames.pushBack(aFrame);
	}

	// play the animation
	auto animation = Animation::createWithSpriteFrames(frames, 0.09f);
	animation->setRestoreOriginalFrame(true);
	moveAnimate = Animate::create(animation);
	moveAnimate->retain();
	

	auto cacher2 = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile( "idle.plist" );
	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames2 = 2;
	Vector<SpriteFrame*> frames2;
	for (int i = 1; i < kNumberOfFrames2; i++)
	{
		std::string s = std::to_string( i );

		SpriteFrame* aFrame2 =
		cacher2->getSpriteFrameByName( "idle_" + s + ".png" );
		frames2.pushBack(aFrame2);
	}

	auto animation2 = Animation::createWithSpriteFrames( frames2, 0.20f );
	animation2->setRestoreOriginalFrame( true) ;
	idleAnimate = Animate::create( animation2 );
	idleAnimate->retain();
	this->runAction( RepeatForever::create( moveAnimate ) );
}

void Player::move( cocos2d::Vec2 direction )
{
	moving = true;

	this->stopAllActions();
	this->runAction( RepeatForever::create( moveAnimate ) );
	this->getPhysicsBody()->setVelocity( direction );

	if( direction.x > 0 )
	{
		dir = 1; //right
	}else{
		dir = 0; //left
	}

}

void Player::idle()
{
	moving = false;

	this->stopAllActions();
	this->runAction(RepeatForever::create( idleAnimate ) );
	this->getPhysicsBody()->setVelocity( cocos2d::Vec2( 0, 0 ) );
}

void Player::update()
{
	//if player is moving
	//cocos2d::Vec2 movement = this->getPhysicsBody()->getVelocity();
	if( moving )
	{
		if( dir == 1 ){ //right
			this->setScaleX( 1 ); //flip
		}else{ //left
			this->setScaleX( -1 ); //flip
		}

	}

}
