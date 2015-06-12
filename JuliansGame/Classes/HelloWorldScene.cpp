#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	//disable for release:
    scene -> getPhysicsWorld() -> setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL ); 

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
	layer -> setPhyschisWorld( scene -> getPhysicsWorld() );

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//screen boundary
	auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );

	auto edgeNode = Node::create();
	edgeNode->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
	edgeNode->setPhysicsBody( edgeBody );

	this->addChild( edgeNode );

	//static-platform
	auto sprite = Sprite::create( "player_blue.png" );
	sprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100) );

	auto spriteBody = PhysicsBody::createBox( sprite->getContentSize(), PhysicsMaterial( 0, 1, 0 ) );
	
	spriteBody->setDynamic(false);
	spriteBody->setCollisionBitmask( 2 );
	spriteBody->setContactTestBitmask( true );
	
	sprite->setPhysicsBody( spriteBody );

	this->addChild( sprite );

	//create enemy-sprites
	auto sprite1 = HelloWorld::createDynamicSprite( "player_red.png", Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	auto sprite2 = HelloWorld::createDynamicSprite( "player_red.png", Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 200));

	{
	//create a sprite declared in header
	mySprite = Sprite::create( "player_blue.png" );
	mySprite->setPosition( Point( visibleSize.width / 2 + origin.x, mySprite->getContentSize().height + origin.y ) );
	auto spriteBody = PhysicsBody::createBox( mySprite->getContentSize(), PhysicsMaterial( 0, 1, 0 ) );

	//spriteBody->setDynamic(false);
	spriteBody->setCollisionBitmask( 2 );
	spriteBody->setContactTestBitmask( true );
	
	mySprite->setPhysicsBody( spriteBody );

	this->addChild( mySprite );
	}

	// listen for contact between objects
	auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( HelloWorld::onContactBegin, this) ;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );

	////touch listener
	//auto event_listener = EventListenerTouchAllAtOnce::create();
	////event_listener ->onTouchesBegan = someFunc();
	//event_listener -> onTouchesEnded = [=](const std::vector<Touch*>& pTouches, Event* event){
	//	auto touch = *pTouches.begin();
	//	auto openGl_location = touch-> getLocation();
	//
	//	auto move_action = MoveTo::create(1.f, openGl_location);
	//	mySprite-> runAction(move_action);
	//	CCLOG("%.1f %.1f", mySprite-> getPositionX(), mySprite-> getPositionY());
	//	};
	//
	//this-> getEventDispatcher()-> addEventListenerWithSceneGraphPriority(event_listener, mySprite);

		//touch listener
	auto event_listener = EventListenerTouchAllAtOnce::create();
	//event_listener ->onTouchesBegan = someFunc();
	event_listener -> onTouchesEnded = [=](const std::vector<Touch*>& pTouches, Event* event){
		auto touch = *pTouches.begin();
		auto openGl_location = touch-> getLocation();
	
		auto move_action = MoveTo::create(1.f, openGl_location);
		

		auto bullet = Sprite::create( "bullet.png" );
		bullet->setPosition( mySprite->getPosition() );
		auto bulletBody = PhysicsBody::createCircle( bullet->getContentSize().width, PhysicsMaterial( 0, 1, 0 ) );
	
		bulletBody->setDynamic(false);
		bulletBody->setCollisionBitmask( 2 );
		bulletBody->setContactTestBitmask( true );
	
		bullet->setPhysicsBody( bulletBody );

		this->addChild( bullet );
		auto callback = CallFunc::create( [this,bullet]() {
			this->actionFinished(bullet);
		});
		auto sequence = Sequence::create(move_action, callback, NULL);
		bullet-> runAction(sequence);
		
		CCLOG("%.1f %.1f", bullet-> getPositionX(), bullet-> getPositionY());
		};
	
	this-> getEventDispatcher()-> addEventListenerWithSceneGraphPriority(event_listener, mySprite);



	//keyboard listener
	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch(keyCode){
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_A:
				event->getCurrentTarget()->setPosition(loc.x-10,loc.y);
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:
				event->getCurrentTarget()->setPosition(loc.x+10,loc.y);
				break;

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			case EventKeyboard::KeyCode::KEY_W:
				event->getCurrentTarget()->setPosition(loc.x,loc.y+10);
				break;

			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			case EventKeyboard::KeyCode::KEY_S:
				event->getCurrentTarget()->setPosition(loc.x,loc.y-10);
				break;

			case EventKeyboard::KeyCode::KEY_SPACE:
				CCJumpTo* jumpTo = CCJumpTo::create(1,event->getCurrentTarget()->getPosition()+Vec2(0,100),1.2f,1);
				 event->getCurrentTarget()->runAction(jumpTo);
				break;
		}

	};
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, mySprite);


    return true;
}

// do something on complete
void HelloWorld::actionFinished(cocos2d::Sprite *bullet) 
{
	this->removeChild(bullet, true);
	CCLOG("action completed!");
}


bool HelloWorld::onContactBegin(cocos2d::PhysicsContact &contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    // check if the bodies have collided
    if ( ( 1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() ) 
		|| ( 2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask() ) )
    {
        CCLOG( "COLLISION HAS OCCURED" );
    }
    
    return true;
}

cocos2d::Sprite* HelloWorld::createDynamicSprite(std::string filename, Vec2 startPosition)
{
	//create a sprite
	auto sprite = Sprite::create( filename );
	sprite->setPosition( Point( startPosition) );

	auto spriteBody = PhysicsBody::createBox( sprite->getContentSize(), PhysicsMaterial( 0, 1, 0 ) );
	
	spriteBody->setCollisionBitmask( 1 );
	spriteBody->setContactTestBitmask( true );
	
	sprite->setPhysicsBody( spriteBody );

	this->addChild( sprite );

	return sprite;
}
