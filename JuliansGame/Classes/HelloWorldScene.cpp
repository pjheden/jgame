#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	//disable for release:
    scene -> getPhysicsWorld() -> setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL ); 
	scene -> getPhysicsWorld() -> setGravity( Vect( 0,0 ) );
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
	//auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
	auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PhysicsMaterial( 0, 0, 0), 3 );
	auto edgeNode = Node::create();
	edgeNode->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
	edgeNode->setPhysicsBody( edgeBody );

	this->addChild( edgeNode );

	//create the player-sprite
	mySprite = Sprite::create( "player_blue.png" );
	mySprite->setPosition( Point( visibleSize.width / 2 + origin.x, mySprite->getContentSize().height + origin.y ) );
	auto spriteBody = PhysicsBody::createBox( mySprite->getContentSize(), PhysicsMaterial( 0, 0, 0 ) );
	spriteBody->setTag( 1 );
	spriteBody->setDynamic(true); //Must be dynamic to be able to set velocity
	

	mySprite->setPhysicsBody( spriteBody );

	this->addChild( mySprite );
	
	//create Énemyseals at random y location
	for(int i = 0; i < 5; ++i)
	{
		HelloWorld::createEnemyseal(visibleSize);
	}



	// listen for contact between objects
	auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( HelloWorld::onContactBegin, this) ;
	contactListener->onContactPreSolve = CC_CALLBACK_2( HelloWorld::onContactPreSolve, this );
	contactListener->onContactPostSolve = CC_CALLBACK_2(HelloWorld::onContactPostSolve,this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );

		//touch listener

	auto event_listener = EventListenerTouchAllAtOnce::create();
	event_listener -> onTouchesEnded = [=](const std::vector<Touch*>& pTouches, Event* event){
		auto touch = *pTouches.begin();
		auto openGl_location = touch-> getLocation();
	
		auto move_action = MoveTo::create(1.f, openGl_location);
		

		auto bullet = Sprite::create( "bullet2.png" );
		bullet->setPosition( mySprite->getPosition() );
		auto bulletBody = PhysicsBody::createBox( bullet->getContentSize(), PhysicsMaterial( 0, 0, 0) );

		bulletBody->setDynamic(true);

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
		auto loc = event->getCurrentTarget()->getPosition();

		switch(keyCode){
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_A:
				event->getCurrentTarget()->setPosition(loc.x-10,loc.y);
				//body->setVelocity( Vect( body->getVelocity().x - 30, body->getVelocity().y) );
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:
				event->getCurrentTarget()->setPosition(loc.x+10,loc.y);
				//body->setVelocity( Vect( body->getVelocity().x + 30, body->getVelocity().y) );
				break;
			case EventKeyboard::KeyCode::KEY_SPACE:
				CCLOG("space");
				break;
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			case EventKeyboard::KeyCode::KEY_W:
				CCLOG("CCJumptTo");
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

void HelloWorld::createEnemyseal(cocos2d::Size visibleSize)
{
	cocos2d::Sprite* sealSprite = Sprite::create( "Seal/seal_walk1.png" );
	sealSprite -> setPosition ( Vec2 ( visibleSize.width, random( 0, (int) visibleSize.height ) ) );
	auto sealBody = PhysicsBody::createBox( sealSprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT); 
	sealBody -> setTag( 2 );
	sealSprite -> setPhysicsBody( sealBody );
	
	sealSprite -> getPhysicsBody() ->setVelocity ( cocos2d::Vect( -10, 0 ) );

	this->addChild( sealSprite );
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


 bool  HelloWorld::onContactPreSolve(cocos2d::PhysicsContact& contact,
PhysicsContactPreSolve& solve) 
 {
		CCLOG( "PreSolve" );
		PhysicsBody* a = contact.getShapeA()->getBody();
        PhysicsBody* b = contact.getShapeA()->getBody();

		//so the player dont bounce on the ground
        if (a->getTag() == mySprite->getTag() || b->getTag() == 
                      mySprite->getTag())
		{
			CCLOG( "Restitution = 0" );
			solve.setRestitution(0);
		}
		return true;
 }

 void HelloWorld::onContactPostSolve(cocos2d::PhysicsContact& contact,
    	const cocos2d::PhysicsContactPostSolve& solve) {
   PhysicsBody* a = contact.getShapeA()->getBody();
   PhysicsBody* b = contact.getShapeA()->getBody();
   PhysicsBody* playerBody = (a->getTag() == mySprite->getTag() )?a:b;
    playerBody->setVelocity(cocos2d::Vect(0,0));
}