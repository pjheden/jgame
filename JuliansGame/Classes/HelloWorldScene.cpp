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
	
	sprite->setPhysicsBody( spriteBody );

	this->addChild( sprite );

	{
	//create a sprite
	auto sprite = Sprite::create( "player_red.png" );
	sprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );

	auto spriteBody = PhysicsBody::createBox( sprite->getContentSize(), PhysicsMaterial( 0, 1, 0 ) );
	
	spriteBody->setCollisionBitmask( 1 );
	spriteBody->setContactTestBitmask( true );
	
	sprite->setPhysicsBody( spriteBody );

	this->addChild( sprite );
	}
	{
	//create a sprite 2 (above)
	auto sprite = Sprite::create( "player_red.png" );
	sprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 200) );

	auto spriteBody = PhysicsBody::createBox( sprite->getContentSize(), PhysicsMaterial( 0, 1, 0 ) );
	
	spriteBody->setCollisionBitmask( 2 );
	spriteBody->setContactTestBitmask( true );
	
	sprite->setPhysicsBody( spriteBody );

	this->addChild( sprite );
	}

	auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( HelloWorld::onContactBegin, this) ;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );

    return true;
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