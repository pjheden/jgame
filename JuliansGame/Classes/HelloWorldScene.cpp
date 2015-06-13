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
	auto sprite0 = HelloWorld::createStaticSprite( "player_blue.png", Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100), PhysicsMaterial( 0, 1, 0 ) );

	//create enemy-sprites
	auto sprite1 = HelloWorld::createDynamicSprite( "player_red.png", Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y), PhysicsMaterial( 0, 1, 0 ) );
	auto sprite2 = HelloWorld::createDynamicSprite( "player_red.png", Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 200), PhysicsMaterial( 0, 1, 0 ) );

	{
	//create a sprite declared in header
	mySprite = Sprite::create( "player_blue.png" );
	mySprite->setPosition( Point( visibleSize.width / 2 + origin.x, mySprite->getContentSize().height + origin.y ) );
	auto spriteBody = PhysicsBody::createBox( mySprite->getContentSize(), PhysicsMaterial( 0, 0, 0 ) );
	//spriteBody->setDynamic(false); //Must be dynamic to be able to set velocity
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
		

		auto bullet = createStaticSprite( "bullet2.png", mySprite->getPosition(), PhysicsMaterial( 0, 1, 0 ) );
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
	eventListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* event)
	{
		switch(keyCode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			event->getCurrentTarget()->getPhysicsBody()->setVelocity( Vect( 0, event->getCurrentTarget()->getPhysicsBody()->getVelocity().y) );
		}



	};

	eventListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event){

		PhysicsBody* body = event->getCurrentTarget()->getPhysicsBody();
		switch(keyCode){
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_A:
				//event->getCurrentTarget()->setPosition(loc.x-10,loc.y);
				body->setVelocity( Vect( body->getVelocity().x - 30, body->getVelocity().y) );
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:
				//event->getCurrentTarget()->setPosition(loc.x+10,loc.y);
				body->setVelocity( Vect( body->getVelocity().x + 30, body->getVelocity().y) );
				break;
			case EventKeyboard::KeyCode::KEY_SPACE:
				CCLOG("space");
				/*auto randEnemy = Sprite::create("red_player.png");
				auto physicBody = PhysicsBody::createBox( randEnemy->getContentSize(), PhysicsMaterial( 0, 1, 0), Vec2( 0, random(0, 50)));
				
				physicBody->setCollisionBitmask( 1 );
				physicBody->setContactTestBitmask ( true );

				randEnemy->setPhysicsBody( physicBody );

				this->addChild( randEnemy );*/
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			case EventKeyboard::KeyCode::KEY_W:
				CCJumpTo* jumpTo = CCJumpTo::create(1, event->getCurrentTarget()->getPosition(), 1.2f, 1);
				event->getCurrentTarget()->runAction(jumpTo);
				CCLOG("Jumped");

				/*if(event->getCurrentTarget()->getActionManager()->getNumberOfRunningActionsInTarget(event->getCurrentTarget()) == 0)
				{
					event->getCurrentTarget()->runAction(jumpTo);
					CCLOG("Jumped");
				}*/
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

cocos2d::Sprite* HelloWorld::createDynamicSprite(std::string filename, cocos2d::Vec2 startPosition, cocos2d::PhysicsMaterial physicsMaterial)
{
	//create a sprite
	auto sprite = Sprite::create( filename );
	sprite->setPosition( Point( startPosition) );

	auto spriteBody = PhysicsBody::createBox( sprite->getContentSize(), physicsMaterial );
	
	spriteBody->setCollisionBitmask( 1 );
	spriteBody->setContactTestBitmask( true );
	
	sprite->setPhysicsBody( spriteBody );

	this->addChild( sprite );

	return sprite;
}

cocos2d::Sprite* HelloWorld::createStaticSprite(std::string filename, cocos2d::Vec2 startPosition, cocos2d::PhysicsMaterial physicsMaterial)
{
	//create a sprite
	auto sprite = Sprite::create( filename );
	sprite->setPosition( Point( startPosition) );

	auto spriteBody = PhysicsBody::createBox( sprite->getContentSize(), physicsMaterial );
	
	spriteBody->setCollisionBitmask( 2 );
	spriteBody->setContactTestBitmask( true );
	spriteBody->setDynamic(false);
	sprite->setPhysicsBody( spriteBody );

	this->addChild( sprite );

	return sprite;
}
