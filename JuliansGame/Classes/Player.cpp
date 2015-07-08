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

	auto pBody = PhysicsBody::createBox( pSprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	pBody->setCollisionBitmask( 1 );
	pBody->setContactTestBitmask( true );
	pSprite->setPhysicsBody( pBody );

    if (pSprite && pSprite->initWithFile( "C:/JuliansGame/JuliansGame/Resources/Indian_idle1.png" ) )
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

PhysicsBody* Player::getBody()
{
	return this->getPhysicsBody();
}


void Player::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
	this->getPhysicsBody()->setVelocityLimit( 120.0f );
}

void Player::addEvents()
{
	///////
	// Keyboard listener
	///////
	auto eventlistener = EventListenerKeyboard::create();
	eventlistener->onKeyPressed = [ this ] ( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event )
	{
		switch (keyCode)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			this->Player::move( Vec2( - this->getPhysicsBody()->getVelocityLimit(), 0 ) );
			break;

		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			this->Player::move( Vec2( this->getPhysicsBody()->getVelocityLimit(), 0 ) );
			break;

		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			this->Player::move( Vec2( 0, - this->getPhysicsBody()->getVelocityLimit()) );
			break;

		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			this->Player::move( Vec2( 0, this->getPhysicsBody()->getVelocityLimit()) );
			break;
		}
	};

	eventlistener->onKeyReleased = [ this ] ( EventKeyboard::KeyCode keyCode, Event* event )
	{
		switch (keyCode)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			this->Player::idle();
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventlistener, this);




	/////
	// If player is touched or not
	/////

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

	//// set the appropriate resource directory for this device
	//FileUtils::getInstance()->addSearchResolutionsOrder("indian_spreedsheet");

	// load and cache the texture and sprite frames
	auto cacher = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile("C:/JuliansGame/JuliansGame/Resources/indian_walk.plist");

	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames = 4;
	Vector<SpriteFrame*> frames;
	for (int i = 1; i < kNumberOfFrames; i++)
	{
		std::string s = std::to_string( i );

		SpriteFrame* aFrame =
		cacher->getSpriteFrameByName( "indian_walk" + s + ".png" );
		frames.pushBack(aFrame);
	}

	// play the animation
	auto animation = Animation::createWithSpriteFrames(frames, 0.40f);
	animation->setRestoreOriginalFrame(true);
	moveAnimate = Animate::create(animation);
	moveAnimate->retain();
	

	auto cacher2 = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile( "indian_idle.plist" );
	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames2 = 4;
	Vector<SpriteFrame*> frames2;
	for (int i = 1; i < kNumberOfFrames2; i++)
	{
		std::string s = std::to_string( i );

		SpriteFrame* aFrame2 =
		cacher2->getSpriteFrameByName( "indian_idle" + s + ".png" );
		frames2.pushBack(aFrame2);
	}

	auto animation2 = Animation::createWithSpriteFrames( frames2, 0.40f );
	animation2->setRestoreOriginalFrame( true) ;
	idleAnimate = Animate::create( animation2 );
	idleAnimate->retain();
	this->runAction( RepeatForever::create( idleAnimate ) );
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
	}else if ( direction.x < 0 ) {
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
		}else if( dir == 0 ){ //left
			this->setScaleX( -1 ); //flip
		}

	}

}