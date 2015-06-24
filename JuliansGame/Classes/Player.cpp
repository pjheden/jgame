#include "Player.h"

using namespace cocos2d;

Player::Player() {}

Player::~Player() 
{
//if retained, must release
	//CC_SAFE_RELEASE(idleAnimate);
	CC_SAFE_RELEASE(moveAnimate);
}

Player* Player::create()
{
    Player* pSprite = new Player();

    if (pSprite->initWithFile( "C:/JuliansGame/JuliansGame/Resources/p1_front.png" ) )
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
	FileUtils::getInstance()->addSearchResolutionsOrder("HD");

	// load and cache the texture and sprite frames
	auto cacher = SpriteFrameCache::getInstance();
	cacher->addSpriteFramesWithFile("p1_spreedsheet.plist");

	#include <sstream>
	// load all the animation frames into an array
	const int kNumberOfFrames = 11;
	Vector<SpriteFrame*> frames;
	for (int i = 1; i < kNumberOfFrames; i++)
	{
		std::string s = std::to_string( i );

		SpriteFrame* aFrame =
		cacher->getSpriteFrameByName( "p1_walk" + s + ".png" );
		frames.pushBack(aFrame);
	}

	// play the animation
	auto animation = Animation::createWithSpriteFrames(frames, 0.05f);
	animation->setRestoreOriginalFrame(true);
	moveAnimate = Animate::create(animation);
	moveAnimate->retain();
	this->runAction( RepeatForever::create( moveAnimate ) );
}
