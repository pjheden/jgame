#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

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

	HelloWorld::mainMenu();
	//HelloWorld::initKeyboardListener();

	//////////////
	//
	//  Seal hunter
	//
	////////////



	////screen boundary
	////auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 );
	//auto edgeBody = PhysicsBody::createEdgeBox( visibleSize, PhysicsMaterial( 0, 0, 0), 3 );
	//auto edgeNode = Node::create();
	//edgeNode->setPosition( Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
	//edgeNode->setPhysicsBody( edgeBody );

	//this->addChild( edgeNode );

	////create the player-sprite
	//mySprite = Sprite::create( "player_blue.png" );
	//mySprite->setPosition( Point( origin.x + mySprite->getContentSize().width, origin.y + visibleSize.height / 2 ) );
	//auto spriteBody = PhysicsBody::createBox( mySprite->getContentSize(), PhysicsMaterial( 0, 0, 1 ) );
	//spriteBody->setCollisionBitmask( 1 );
	//spriteBody->setContactTestBitmask( true );
	//spriteBody->setDynamic( true ); //Must be dynamic to be able to set velocity
	//spriteBody->setVelocityLimit( 60 ); //Max player movement speed

	//mySprite->setPhysicsBody( spriteBody );

	//this->addChild( mySprite );
	//
	////create Enemyseals at random y location
	//for(int i = 0; i < 5; ++i)
	//{
	//	HelloWorld::createEnemyseal(visibleSize);
	//}



	//// listen for contact between objects
	//auto contactListener = EventListenerPhysicsContact::create();
 //   contactListener->onContactBegin = CC_CALLBACK_1( HelloWorld::onContactBegin, this) ;
 //   this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );

    return true;
}

// do something on complete
void HelloWorld::actionFinished(cocos2d::Sprite *bullet) 
{
	this->removeChild(bullet, true);
	//CCLOG("shot completed!");
}

void HelloWorld::createEnemyseal(cocos2d::Size visibleSize)
{
	cocos2d::Sprite* sealSprite = Sprite::create( "Seal/seal_walk1.png" );
	sealSprite -> setPosition ( Vec2 ( visibleSize.width, random( 0, (int) visibleSize.height ) ) );
	auto sealBody = PhysicsBody::createBox( sealSprite->getContentSize(), PhysicsMaterial() ); 
	sealBody->setCollisionBitmask(2);
    sealBody->setContactTestBitmask(true);
	sealBody -> setDynamic( true );

	sealSprite -> setPhysicsBody( sealBody );
	
	sealSprite -> getPhysicsBody() ->setVelocity ( cocos2d::Vect( -10, 0 ) );

	this->addChild( sealSprite );
}



bool HelloWorld::onContactBegin(cocos2d::PhysicsContact &contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
	// check if the bodies have collided

	//if player collide with seal
    if ( ( 1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() ) 
		|| ( 2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask() ) )
    {
        CCLOG( "Player collide with seal" );
    }
	//if seal collide with bullet
	else if( ( 2 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask() ) 
		|| ( 3 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() ) )
	{
		if( a->getCollisionBitmask() == 2)
		{
			a->getNode()->removeFromParent();
		}else{
			b->getNode()->removeFromParent();
		}
		CCLOG( "Seal collide with bullet" );
	}

    
    return true;
}

bool HelloWorld::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{
	if(touch->getLocation().x < pSprite->getPositionX())
	{
		pSprite->move( Vec2( -60, 0 ) ); // left
	}
	if(touch->getLocation().x > pSprite->getPositionX())
	{
		pSprite->move( Vec2( 60, 0 ) ); // right
	}
 
	return true;
}
 
void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
	pSprite->idle();
}

//called for every update of the game
void HelloWorld::update(float dt)
{
	pSprite->update();
}

//exit the game
void HelloWorld::menuCloseCallback( cocos2d::Ref* pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
 
    Director::getInstance()->end();
 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::startGame( cocos2d::Ref* pSender )
{
	HelloWorld::gameMenu();

	//game
	//////////////
	//
	//  Player movement with animation
	//
	////////////

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//start game
	this->scheduleUpdate();

	pSprite = Player::create();
	pSprite->setPosition( Point( origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 ) );
	this->addChild(pSprite, 5); //second parameter is the drawing priority

	HelloWorld::initShooting();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
 
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
 
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}

void HelloWorld::settings( cocos2d::Ref* pSender )
{

	HelloWorld::settingsMenu();
}

void HelloWorld::mainMenu()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuTitle = MenuItemFont::create( "Game Name" );
	menuTitle->setPosition( Vec2( origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - menuTitle->getContentSize().height) );

	auto playTitle = MenuItemFont::create( "Play", this, menu_selector( HelloWorld::startGame ) );
	playTitle->setPosition( Vec2( origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2 ) );

	auto settingsTitle = MenuItemFont::create( "Settings", this, menu_selector( HelloWorld::settings ) );
	settingsTitle->setPosition( Vec2( origin.x + visibleSize.width / 2,
		playTitle->getPosition().y - 2 * settingsTitle->getContentSize().height ) );

	auto closeItem = MenuItemFont::create( "Exit", this, menu_selector(HelloWorld::menuCloseCallback));    
	closeItem->setPosition( Vec2( origin.x + visibleSize.width / 2,
		settingsTitle->getPosition().y - 2 * closeItem->getContentSize().height ) );

	auto menu = Menu::create(closeItem, menuTitle, playTitle, settingsTitle, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setName( "mainMenu" );
	this->addChild(menu, 1);
}
void HelloWorld::removeMainMenu()
{
	this->removeChildByName( "mainMenu" );
}

void HelloWorld::settingsMenu()
{
	HelloWorld::removeMainMenu();

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto slider = ui::Slider::create();
	slider->setPosition( origin + visibleSize / 2 );
	slider->loadBarTexture("Slider_Back.png"); // what the slider looks like
	slider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png");
	slider->loadProgressBarTexture("Slider_PressBar.png");
	slider->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
			switch (type)
			{
					case ui::Widget::TouchEventType::BEGAN:
							break;
					case ui::Widget::TouchEventType::ENDED:
							CCLOG( "Slider moved" );
							break;
					default:
							break;
			}
	});
	slider->setName( "slider" );
	this->addChild( slider );

	auto soundText = ui::TextField::create( "Sound", "fonts/Marker Felt.ttf", 30 );
	soundText->setPosition( Vec2( slider->getPosition().x - slider->getContentSize().width / 2 -  soundText->getContentSize().width,
		slider->getPosition().y ));
	soundText->setName( "soundText" );

	this->addChild( soundText );

	// kan annars ta bort denna texten och ha png med text på, för knappen under
	auto buttonText = ui::TextField::create( "Back", "fonts/Marker Felt.ttf", 30 );
	buttonText->setPosition( Vec2( slider->getPosition().x,
		slider->getPosition().y - slider->getContentSize().height - 2 * buttonText->getContentSize().height ) );
	buttonText->setName( "buttonText" );

	this->addChild( buttonText );

	auto backButton = ui::Button::create( "Button_Normal.png", "Button_Press.png", "Button_Disable.png" );

	backButton->setPosition( Vec2( buttonText->getPosition().x,
		buttonText->getPosition().y - buttonText->getContentSize().height - backButton->getContentSize().height / 2 ) );
	backButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
                case ui::Widget::TouchEventType::BEGAN:
                        break;
                case ui::Widget::TouchEventType::ENDED:
                        HelloWorld::removeSettingsMenu();
                        break;
                default:
                        break;
        }
	});
	backButton->setName( "backButton" );
	this->addChild(backButton);

}
void HelloWorld::removeSettingsMenu()
{
	this->removeChildByName( "slider" );
	this->removeChildByName( "soundText" );
	this->removeChildByName( "backButton" );
	this->removeChildByName( "buttonText" );

	HelloWorld::mainMenu();
}

void HelloWorld::playMenu()
{
	HelloWorld::removeMainMenu();
}
void HelloWorld::removePlayMenu()
{
	this->removeChildByName( "playMenu" );
}

void HelloWorld::gameMenu()
{
	HelloWorld::removeMainMenu();

	//score
	//weapon indicator
	//level
}

void HelloWorld::removeGameMenu()
{
	this->removeChildByName( "gameMenu" );
}



void HelloWorld::initShooting()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//keyboard listener
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [ this, origin, visibleSize]( EventKeyboard::KeyCode keyCode, Event* event ){

		PhysicsBody* body = event->getCurrentTarget()->getPhysicsBody();

		switch(keyCode){
			case EventKeyboard::KeyCode::KEY_SPACE:
				//shoot a bullet straight forward
				auto bulletSpeed = 300.0f;
				auto distance = origin.x + visibleSize.width - pSprite->getPosition().x;
				auto move_action = MoveTo::create( distance / bulletSpeed, Vec2( origin.x + visibleSize.width, pSprite->getPosition().y ) );

				auto bullet = Sprite::create ( "arrow.png" );
				bullet->setPosition( pSprite->getPosition() + bullet->getContentSize() + Vec2( 10, 0 ) );
				auto bulletBody = PhysicsBody::createBox( bullet->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

				bulletBody->setDynamic( true );
				bulletBody->setCollisionBitmask( 3 );
				bulletBody->setContactTestBitmask( true );

				bullet->setPhysicsBody( bulletBody );
				this->addChild( bullet );

				auto callback = CallFunc::create( [this,bullet]() { this->actionFinished(bullet); } );
				auto sequence = Sequence::create(move_action, callback, NULL);
				bullet-> runAction(sequence);
		
					

				break;

		}

	};
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, pSprite);
}

