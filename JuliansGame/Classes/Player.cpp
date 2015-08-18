#include "Player.h"
#include "GameController.h"
#include "stdReplacer.h"

using namespace cocos2d;
cocos2d::PhysicsBody* pBody;

Player::Player() {}

Player::~Player() 
{
//if retained, must release
	CC_SAFE_RELEASE(idleAnimate);
	CC_SAFE_RELEASE(moveAnimate);
	//CC_SAFE_RELEASE(shootAnimate);
}

Player* Player::create()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

    Player* pSprite = new Player();
	pSprite->nrOfArrows = 3;
	pSprite->initWithFile( "Indian_idle1.png" );
	pSprite->setScaleX( ( visibleSize.width / pSprite->getBoundingBox().size.width) * 1/12 );
	pSprite->setScaleY( ( visibleSize.height / pSprite->getBoundingBox().size.height ) * 1/11 );

	pBody = PhysicsBody::createBox( pSprite->getBoundingBox().size, PHYSICSBODY_MATERIAL_DEFAULT);

	//pBody->setCategoryBitmask( 1 );
	pBody->setCollisionBitmask( 1 );
	pBody->setContactTestBitmask( true );

	pBody->setRotationEnable( false );
	//pSprite->setPhysicsBody( pBody );

    if ( pSprite )
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
	return pBody;
}

void Player::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
	pBody->setVelocityLimit( 120.0f );
	pBody->setRotationEnable( false );
}

void Player::addEvents()
{
	///////
	// Keyboard listener
	///////
	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC	
		auto eventlistener = EventListenerKeyboard::create();
		eventlistener->onKeyPressed = [ this ] ( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event )
		{
			switch (keyCode)
			{
			/*case cocos2d::EventKeyboard::KeyCode::KEY_A:
				this->Player::move( Vec2( - pBody->getVelocityLimit(), 0 ) );
				break;

			case cocos2d::EventKeyboard::KeyCode::KEY_D:
				this->Player::move( Vec2( pBody->getVelocityLimit(), 0 ) );
				break;*/

			case cocos2d::EventKeyboard::KeyCode::KEY_S:
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				this->Player::move( Vec2( 0, - pBody->getVelocityLimit()) );
				break;

			case cocos2d::EventKeyboard::KeyCode::KEY_W:
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
				this->Player::move( Vec2( 0, pBody->getVelocityLimit()) );
				break;

			case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
				//this->stopAllActions();
				//this->runAction( shootAnimate  );
				this->Player::shoot();
				break;
			}
		};

		eventlistener->onKeyReleased = [ this ] ( EventKeyboard::KeyCode keyCode, Event* event )
		{
			switch (keyCode)
			{
			//case cocos2d::EventKeyboard::KeyCode::KEY_A:
			//case cocos2d::EventKeyboard::KeyCode::KEY_D:
			case cocos2d::EventKeyboard::KeyCode::KEY_S:
			case cocos2d::EventKeyboard::KeyCode::KEY_W:
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				this->Player::idle();
			}
		};

		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventlistener, this);
	#else
		//mobile phone, set touch
		auto eventListener = EventListenerTouchOneByOne::create();
		eventListener->onTouchBegan = CC_CALLBACK_2(Player::onTouchBegan, this);
		eventListener->onTouchEnded = CC_CALLBACK_2(Player::onTouchEnded, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

	#endif
}


void Player::initAnimations()
{

	//// set the appropriate resource directory for this device
	//FileUtils::getInstance()->addSearchResolutionsOrder("indian_spreedsheet");

	// load and cache the texture and sprite frames
	auto cacher = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile("indian_walk.plist");

	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames = 4;
	Vector<SpriteFrame*> frames;
	for (int i = 1; i <= kNumberOfFrames; i++)
	{
		std::string s = stdReplacer::to_string( i );

		SpriteFrame* aFrame =
		cacher->getSpriteFrameByName( "indian_walk" + s + ".png" );
		frames.pushBack(aFrame);
	}

	// play the animation
	auto animation = Animation::createWithSpriteFrames(frames, 0.40f);
	animation->setRestoreOriginalFrame(true);
	moveAnimate = Animate::create(animation);
	moveAnimate->retain();
	
	cacher->addSpriteFramesWithFile( "indian_idle.plist" );
	// load all the animation frames into an array
	const int kNumberOfFrames2 = 4;
	Vector<SpriteFrame*> frames2;
	for (int i = 1; i <= kNumberOfFrames2; i++)
	{
		std::string s = stdReplacer::to_string( i );

		SpriteFrame* aFrame2 =
		cacher->getSpriteFrameByName( "indian_idle" + s + ".png" );
		frames2.pushBack(aFrame2);
	}

	auto animation2 = Animation::createWithSpriteFrames( frames2, 0.40f );
	animation2->setRestoreOriginalFrame( true) ;
	idleAnimate = Animate::create( animation2 );
	idleAnimate->retain();

	//cacher->addSpriteFramesWithFile( "indian_shoot.plist" );
	//// load all the animation frames into an array
	//const int kNumberOfFrames3 = 4;
	//Vector<SpriteFrame*> frames3;
	//for (int i = 1; i < kNumberOfFrames3; i++)
	//{
	//	std::string s = stdReplacer::to_string( i );

	//	SpriteFrame* aFrame3 =
	//	cacher->getSpriteFrameByName( "indian_shoot" + s + ".png" );
	//	frames3.pushBack( aFrame3 );
	//}

	//auto animation3 = Animation::createWithSpriteFrames( frames3, 0.40f );
	//animation3->setRestoreOriginalFrame( true) ;
	//shootAnimate = Animate::create( animation3 );
	//shootAnimate->retain();

	this->runAction( RepeatForever::create( idleAnimate ) );

}

void Player::move( cocos2d::Vec2 direction )
{
	moving = true;

	this->stopAllActions();
	auto action = RepeatForever::create( moveAnimate ) ;
	action->setTag( 1 );
	this->runAction( action );
	pBody->setVelocity( direction );

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
	
	pBody->setVelocity( cocos2d::Vec2( 0, 0 ) );
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

void Player::shoot()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto tar = Vec2( origin.x + visibleSize.width, this->getPosition().y );
	
    GameController::spawnBullet( 1, Vec2( this->getPosition().x + this->getBoundingBox().size.width / 2, this->getPosition().y ) , tar );
}

bool Player::onTouchBegan( Touch *touch, Event *unused_event )
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto touchLoc = touch->getLocation();
	if(touchLoc.x >= visibleSize.width / 2)
	{
		//shoot
		this->Player::shoot();
	}else
	{
		if(touchLoc.y >= visibleSize.height / 2)
		{
			//walk up
			this->Player::move( Vec2( 0, pBody->getVelocityLimit()) );
		}else
		{
			// walk down
			this->Player::move( Vec2( 0, - pBody->getVelocityLimit()) );
		}
		
	}
	return true;
}

void Player::onTouchEnded( Touch *touch, Event *unused_event )
{
	this->Player::idle();
}
