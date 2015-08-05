#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

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
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if ( !audio->isBackgroundMusicPlaying() )
	{
		audio->playBackgroundMusic( "sky-loop.wav", true );
	}

    /*Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();*/
	HelloWorld::mainMenu();
	
    return true;
}

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

void HelloWorld::settings( cocos2d::Ref* pSender )
{
	HelloWorld::settingsMenu();
}
void HelloWorld::gameInit( cocos2d::Ref* pSender )
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if ( audio->isBackgroundMusicPlaying() )
	{
		audio->stopBackgroundMusic();
	}
	auto scene = GameLayer::createScene();
	Director::getInstance()->pushScene( scene );
}



void HelloWorld::mainMenu()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuTitle = MenuItemFont::create( "White Man Came" );
	menuTitle->setPosition( Vec2( origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - menuTitle->getContentSize().height) );

	auto playTitle = MenuItemFont::create( "Play", this, menu_selector( HelloWorld::gameInit ) );
	playTitle->setPosition( Vec2( origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2 ) );

	auto settingsTitle = MenuItemFont::create( "Settings", this, menu_selector( HelloWorld::settings ) );
	settingsTitle->setPosition( Vec2( origin.x + visibleSize.width / 2,
		playTitle->getPosition().y - 2 * settingsTitle->getContentSize().height ) );

	auto closeItem = MenuItemFont::create( "Exit", this, menu_selector(HelloWorld::menuCloseCallback));    
	closeItem->setPosition( Vec2( origin.x + visibleSize.width / 2,
		settingsTitle->getPosition().y - 2 * closeItem->getContentSize().height ) );

	auto testItem = MenuItemFont::create( "Test", this, menu_selector( HelloWorld::gameTest ) );
	testItem->setPosition( Vec2( origin.x + visibleSize.width / 2,
		closeItem->getPosition().y -2 * testItem->getContentSize().height ) );

	auto menu = Menu::create(closeItem, menuTitle, playTitle, settingsTitle, testItem, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setName( "mainMenu" );
	this->addChild(menu);
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
	slider->loadBarTexture( "Slider_Back.png" ); // what the slider looks like
	slider->loadSlidBallTextures( "SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png" );
	slider->loadProgressBarTexture( "Slider_PressBar.png" );
	slider->addTouchEventListener( [ & ]( Ref* sender, ui::Widget::TouchEventType type ){
			switch ( type )
			{
					case ui::Widget::TouchEventType::BEGAN:
							break;
					case ui::Widget::TouchEventType::ENDED:
							auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
							audio->setEffectsVolume( slider->getPercent() / 100 );
							audio->setBackgroundMusicVolume( slider->getPercent() / 100 );
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







void HelloWorld::gameTest( cocos2d::Ref* pSender )
{
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite1 = Sprite::create( "player_blue.png" );
	auto body1 = PhysicsBody::createBox( sprite1->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

	body1->setCategoryBitmask(10);    
	body1->setCollisionBitmask(2);  
	body1->setContactTestBitmask( 0 );

	sprite1->setPhysicsBody( body1 );
	sprite1->setPosition( origin.x +  2* sprite1->getContentSize().width, origin.y + visibleSize.height / 2);

	this->addChild(sprite1);


	auto sprite2 = Sprite::create( "player_blue.png" );
	auto body2 = PhysicsBody::createBox( sprite1->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

	body2->setCategoryBitmask(1);   
	body2->setCollisionBitmask(10);  
	body2->setContactTestBitmask( 0 );

	sprite2->setPhysicsBody( body2 );
	sprite2->setPosition( origin.x +  2* sprite2->getContentSize().width + 100, origin.y + visibleSize.height / 2);

	this->addChild(sprite2);

	auto sprite3 = Sprite::create( "player_red.png" );
	auto body3 = PhysicsBody::createBox( sprite1->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

	body3->setCategoryBitmask(10);    // 0010
	body3->setCollisionBitmask(1);   // 0001
	body3->setContactTestBitmask( 0 );


	sprite3->setPhysicsBody( body3 );
	sprite3->setPosition( origin.x +  2* sprite2->getContentSize().width, origin.y + visibleSize.height / 2 - 100);

	this->addChild(sprite3);

	auto sprite4 = Sprite::create( "player_red.png" );
	auto body4 = PhysicsBody::createBox( sprite1->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );

	body4->setCategoryBitmask(10);    // 0010
	body4->setCollisionBitmask(1);   // 0001
	body4->setContactTestBitmask( 0 );

	sprite4->setPhysicsBody( body4);
	
	sprite4->setPosition( origin.x +  2* sprite2->getContentSize().width + 100, origin.y + visibleSize.height / 2 - 100);

	this->addChild(sprite4);


	sprite3->getPhysicsBody()->setVelocity( Vec2( 10, 0 ) );

	sprite1->setPosition( sprite4->getPosition().x + 100, sprite4->getPosition().y );
	sprite2->setPosition( sprite1->getPosition().x + 100, sprite4->getPosition().y );

	sprite1->getPhysicsBody()->setVelocity( Vec2( -10, 0 ) );

	
}

bool HelloWorld::onContactBegan(PhysicsContact &contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA->getPhysicsBody()->getCategoryBitmask() == 10 & nodeB->getPhysicsBody()->getCategoryBitmask() == 9 
		|| nodeB->getPhysicsBody()->getCategoryBitmask() == 10 & nodeA->getPhysicsBody()->getCategoryBitmask() == 9 )
	{

	}
	return true;
}
