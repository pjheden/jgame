#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"



Scene* MainMenu::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::create();
	//disable for release:
    //scene -> getPhysicsWorld() -> setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL ); 
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();
	layer -> setPhyschisWorld( scene -> getPhysicsWorld() );

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MainMenu::init()
{
	
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getWinSize();

	//audio settings
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if ( !audio->isBackgroundMusicPlaying() )
	{
		audio->setBackgroundMusicVolume( 0.5f );
		audio->playBackgroundMusic( "sky-loop.wav", true );
	}

	//add background
	cocos2d::Sprite* background = Sprite::create( "mainmenu.png" );
	background->setScaleX((visibleSize.width / background->getContentSize().width));
	background->setScaleY((winSize.height / background->getContentSize().height));
	background->setAnchorPoint( Vec2( origin.x, origin.y ) );
    //background->setPosition( origin.x + visibleSize.width / 2, origin.y + visibleSize.height );
    this->addChild( background, -1, -1 );
    background->setGlobalZOrder( -1 );

	//adds start game
	cocos2d::Sprite* start_normal=Sprite::create( "startgame1.png" );
    cocos2d::Sprite* start_pressed=Sprite::create( "startgame2.png" );
    startgame_item = MenuItemSprite::create(start_normal, start_pressed, CC_CALLBACK_1(MainMenu::startgame, this));
    startgame_item->setPosition( visibleSize.width/2 ,200 );
	
	//adds settings
	cocos2d::Sprite* settings_normal=Sprite::create( "settings1.png" );
    cocos2d::Sprite* settings_pressed=Sprite::create( "settings2.png" );
    settings_item = MenuItemSprite::create(settings_normal, settings_pressed, CC_CALLBACK_1(MainMenu::settings, this));
    settings_item->setPosition( visibleSize.width/2 , 100 );

	//************* Menu ******************
    auto menu = Menu::create(startgame_item, settings_item, NULL);
    menu->setPosition(origin);
	menu->setName("menu");
    this->addChild(menu,3);

	
	return true;
}

void MainMenu::startgame(Ref* sender)
{
	auto scene = GameLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MainMenu::settings(Ref* sender)
{
	this->removeChildByName( "menu" );
	auto layer = Layer::create();

	Size visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	
	//adds slider for volum
	auto slider = ui::Slider::create();
	slider->setTouchEnabled( true );
	slider->loadBarTexture( "Slider_Back.png" );
	slider->loadSlidBallTextures( "SliderNode_Normal.png", "SliderNode_Normal.png", "" );
	slider->loadProgressBarTexture( "Slider_PressBar.png" );
	slider->setPercent( audio-> getBackgroundMusicVolume() * 100 );
	slider->setPosition( Vec2( origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 ) );
	
	slider->addEventListener( CC_CALLBACK_2( MainMenu::settingsSliderCallFunc, this ) );
	layer->addChild( slider );

	//adds back button
	cocos2d::Sprite* back_normal=Sprite::create( "back1.png" );
    cocos2d::Sprite* back_pressed=Sprite::create( "back2.png" );
    back_item = MenuItemSprite::create(back_normal, back_pressed, CC_CALLBACK_1(MainMenu::back, this));
    back_item->setPosition( visibleSize.width/2 ,200 );

	auto menu = Menu::create(back_item, NULL);
    menu->setPosition(origin);
	layer->setName( "layer" );
    layer->addChild(menu,3);

	this->addChild( layer );
}

void MainMenu::back(Ref* sender)
{
	this->removeChildByName( "layer");
	MainMenu::init();
}


void MainMenu::settingsSliderCallFunc(Ref* pSender, ui::Slider::EventType type)
{
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        ui::Slider* slider = dynamic_cast<ui::Slider*>(pSender);
        int percent = slider->getPercent();

		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->setBackgroundMusicVolume( (float) percent / 100 );
		audio->setEffectsVolume( (float) percent / 100 );
	}

}