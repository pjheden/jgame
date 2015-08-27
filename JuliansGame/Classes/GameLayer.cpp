#include "GameLayer.h"
#include "GameController.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"
#include "stdReplacer.h"

USING_NS_CC;
const char* HIGH_SCORE="key1";
const char* NAME_SCORE="key2";

bool GameLayer::_dead = false;
Scene* GameLayer::_gameScene;

GameLayer::GameLayer(void){}
GameLayer::~GameLayer(void){}

cocos2d::Scene* GameLayer::createScene()
{
	_gameScene = Scene::createWithPhysics();
	_gameScene -> getPhysicsWorld() -> setGravity( Vect( 0, 0 ) );
   // _gameScene -> getPhysicsWorld() -> setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL ); 

	auto layer = GameLayer::create();
	layer -> setPhyschisWorld( _gameScene -> getPhysicsWorld() );


    _gameScene->addChild(layer);

    return _gameScene;
}

bool GameLayer::init()
{
	 if ( !Layer::init() ) return false;

	//init the music
	 auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	 if( !audio->isBackgroundMusicPlaying() )
	 {
		audio -> playBackgroundMusic( "background.wav", true );
	 }
	 //audio->playEffect( "gunshot.wav", false, 1.0f, 1.0f, 1.0f );

	 GameLayer::initGame();

	// listen for contact between objects
	auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( GameLayer::onContactBegin, this );
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );

	//init the schedule for the game, main loop
    schedule( schedule_selector( GameLayer::update ) );

	return true;
}

void GameLayer::update( float dt )
{
	if(_dead)
	{
		return;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();


	if( GameController::_player->getPosition().y + GameController::_player->getBoundingBox().size.height / 2 > visibleSize.height )
	{
		GameController::_player->Player::idle();
		GameController::_player->setPosition( Vec2( GameController::_player->getPosition().x,
			visibleSize.height - GameController::_player->getBoundingBox().size.height / 2  ) );
	}
	else if( GameController::_player->getPosition().y - GameController::_player->getBoundingBox().size.height / 2 < 0 )
	{
		GameController::_player->Player::idle();
		GameController::_player->setPosition( Vec2( GameController::_player->getPosition().x,
			GameController::_player->getBoundingBox().size.height / 2  ) );
	}


	//Update bullets
	 GameLayer::bulletControl();
	
	 //update arrow counter
	 GameLayer::updateArrows();

	//spawn enemies
	 GameLayer::spawnEnemies();

	//check raycasts
	while( GameController::rayStart.size() > 0 )
	{
		Vec2 start = GameController::rayStart.at( 0 );
		Vec2 end = GameController::rayEnd.at( 0 );
		auto node = DrawNode::create();
		node->drawSegment( start, end, 1, cocos2d::Color4F::RED );
		GameLayer::_gameScene->addChild( node );
		auto time = DelayTime::create( 3.0f );
		auto ray = CallFunc::create( [this, start, end]() { this->rCast( start, end ); } );
		auto callback = CallFunc::create( [this, node]() { this->removeCast( node ); } );
		auto action = Sequence::create( time, callback, ray, nullptr );
		node->runAction( action );
		GameController::rayStart.erase( GameController::rayStart.begin() );
		GameController::rayEnd.erase( GameController::rayEnd.begin() );

	}

}

void GameLayer::spawnEnemies()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	EnemyCB* cb;
	EnemyCB* sniper;
	if(GameController::enemies.size() < 5)
	{
		cb = GameController::spawnEnemy( 2 ); //int 2=cowboy, 3=worker

		if( cb )
		{
			addChild( cb, 2);
		}

		sniper = GameController::spawnEnemy( 4 ); //int 2=cowboy, 3=worker, 4 = sniper
		if( sniper )
		{
			addChild( sniper, 2 );
		}
	}
}

void GameLayer::removeCast( DrawNode* node )
{
	_gameScene->removeChild( node );
	//GameController::casts.erase( GameController::casts.begin() );
	//n->getNode()->removeFromParentAndCleanup( true );
}

void GameLayer::bulletControl()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bulletSpeed = 300.0f;
	
	Bullet* b;
	for(int i = GameController::bullets.size()-1; i >= 0; i-- )
	{
		b = GameController::bullets.at ( i );
		auto parent = b->getParent();
		if(!parent)
		{
			addChild( b );
			if(b->getTarget() != b->getPosition() )
			{
				int type = b->getType();
				float distance = 0.0f;
				switch( type )
				{
				case 1: //indian - arrow
					distance = origin.x + visibleSize.width - b->getPosition().x;
					break;
				case 2: //cowboy - bullet
					distance = b->getPosition().x - origin.x;
					break;
				}
				auto move_action = MoveTo::create( distance / b->getSpeed().x, b->getTarget() );
				auto callback = CallFunc::create( [this, b]() { this->removeBullet( b ); } );
				auto sequence = Sequence::create( move_action, callback, NULL );
				b-> runAction(sequence);
			}
		}		
	}
}

void GameLayer::removeBullet( Bullet *b ) 
{
	if ( b->getType() == 1 )
	{
		GameController::_player->nrOfArrows += 1;

	}

	GameController::bullets.eraseObject( b );
	b->cleanup();
	removeChild(b, true);
}

bool GameLayer::onContactBegin ( cocos2d::PhysicsContact &contact )
{
	PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();

	// check if the bodies have collided

	//if player collide with Worker,Cowboy
    if ( ( 1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() ) 
		|| ( 2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask() ) )
    {
        CCLOG( "Player collide with Worker / Cowboy" );
    }
	//if Worker,Cowboy collide with arrow
	else if( ( 2 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask() ) 
		|| ( 3 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() ) )
	{
		GameController::erase( a->getNode() );
		GameController::erase( b->getNode() );
		GameLayer::updateScore( 30 );
	}
	//if player collide with bullet
	else if( ( 1 == a->getCollisionBitmask() && 4 == b->getCollisionBitmask() ) 
		|| ( 4 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask() ) )
	{
		GameLayer::playerDead();
	}

    return true;
}

void GameLayer::playerDead()
{
	GameLayer::_dead = true;
	GameController::eraseAll();
	GameLayer::lostLayer();
}

void GameLayer::updateScore( int nr )
{
	Label* scoreLabel = (Label*) 	_gameScene->getChildByName( "scoreInt" );
	if(scoreLabel)
	{
		auto str = scoreLabel->getString();
		int tempScore = stdReplacer::stoi( str );
		scoreLabel->setString( stdReplacer::to_string( tempScore + nr ) );
	}
}

void GameLayer::updateArrows()
{
	int nr = GameController::_player->nrOfArrows + 1;
	Label* arrowLabel = (Label*) _gameScene->getChildByName( "nrArrows" );
	if(arrowLabel)
	{
		arrowLabel->setString( std::string( "Arrows: " ) + stdReplacer::to_string( nr ) + std::string( " / 4" ));
	}
}

void GameLayer::lostLayer()
{
	//Can make this so it isnt remade everytime, but instead retained?
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto counterSize =  _gameScene->getChildByName( "nrArrows" )->getBoundingBox().size;
	_gameScene->removeChildByName( "nrArrows" );

	_lostLayer = Layer::create();
	_lostLayer->setName( "lostLayer" );

	auto lostText = Label::create( "Game Over!", "fonts/Marker Felt.ttf", 60);
	lostText->setName("lostText");
	lostText->setTextColor( cocos2d::Color4B::BLACK );
	lostText->setScaleX( (visibleSize.width / lostText->getBoundingBox().size.width) * 1/2 );
	lostText->setScaleY( (visibleSize.height / lostText->getBoundingBox().size.height) * 1/8 );
	lostText->setPosition( Vec2( origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - counterSize.height - lostText->getBoundingBox().size.height ) );
	_lostLayer->addChild( lostText );

	//adds retry button
	cocos2d::Sprite* retry_normal=Sprite::create( "retrybutton1.png" );
    cocos2d::Sprite* retry_pressed=Sprite::create( "retrybutton2.png" );
    retry_item = MenuItemSprite::create( retry_normal, retry_pressed, CC_CALLBACK_1( GameLayer::retryGame, this ) );
	retry_item->setScaleX( (visibleSize.width / retry_item->getBoundingBox().size.width) * 1/2 );
	retry_item->setScaleY( (visibleSize.height / retry_item->getBoundingBox().size.height) * 1/8 );
    retry_item->setPosition( Vec2( origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2 - 2 * retry_item->getBoundingBox().size.height ) );

	//adds menu button
	cocos2d::Sprite* menu_normal=Sprite::create( "menubutton1.png" );
    cocos2d::Sprite* menu_pressed=Sprite::create( "menubutton2.png" );
    menu_item = MenuItemSprite::create( menu_normal, menu_pressed, CC_CALLBACK_1( GameLayer::backToMenu, this ) );
	menu_item->setScaleX( (visibleSize.width / menu_item->getBoundingBox().size.width) * 1/2 );
	menu_item->setScaleY( (visibleSize.height / menu_item->getBoundingBox().size.height) * 1/8 );
    menu_item->setPosition( Vec2( origin.x + visibleSize.width / 2,
		retry_item->getPosition().y - menu_item->getBoundingBox().size.height ) );

	auto menu = Menu::create( retry_item, menu_item, NULL );
	menu->setPosition(Vec2::ZERO);
	menu->setName( "menu" );
	_lostLayer->addChild( menu );

	GameLayer::checkHighscore();
	GameLayer::_gameScene->addChild( _lostLayer );
}

void GameLayer::checkHighscore()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto def = CCUserDefault::sharedUserDefault();
	auto playerScore = (Label* )  _gameScene->getChildByName( "scoreInt" ); //get the actual score

	//draws the highscore
	char score[20];
	sprintf( score, "%d", def->getIntegerForKey( HIGH_SCORE ) );

	std::string printString( "Highscore:   " );
	printString += def->getStringForKey( NAME_SCORE );
	printString += " ";
	printString += score;

	auto highScore = Label::create( printString , "fonts/Marker Felt.ttf", 30 );
	highScore->setScaleX( ( visibleSize.width / highScore->getBoundingBox().size.width ) * 1/3 );
	highScore->setScaleY( ( visibleSize.height / highScore->getBoundingBox().size.height ) * 1/8 );
	highScore->setTextColor( cocos2d::Color4B::BLACK );
	highScore->setPosition( Vec2( origin.x + visibleSize.width / 2, origin.y + visibleSize.height - highScore->getBoundingBox().size.height / 2 ) );
	_lostLayer->addChild( highScore );


	//compare the scores
	if( stdReplacer::stoi(playerScore->getString() ) > def->getIntegerForKey( HIGH_SCORE ) )
	//if(true)
	{
		_lostLayer->getChildByName( "menu" )->setVisible( false );
		//new highscore
		_highscoreLayer = Layer::create();
		_highscoreLayer->setName( "highscoreLayer" );

		auto background = Sprite::create( "highscorebox.png" );
		background->setScaleX( ( visibleSize.width / background->getBoundingBox().size.width ) * 1/2 );
		background->setScaleY( ( visibleSize.height / background->getBoundingBox().size.height ) * 1/2 );
		background->setPosition( Vec2( origin.x + visibleSize.width / 2,
			_lostLayer->getChildByName( "lostText" )->getPosition().y - _lostLayer->getChildByName( "lostText" )->getBoundingBox().size.height / 2
			 - background->getBoundingBox().size.height / 2 - 10 ) );

		_highscoreLayer->addChild( background );

		auto newScore = Label::create( "New Highscore!" , "fonts/Marker Felt.ttf", 30 );
		newScore->setScaleX( ( visibleSize.width / newScore->getBoundingBox().size.width ) * 1/3 );
		newScore->setScaleY( ( visibleSize.height / newScore->getBoundingBox().size.height ) * 1/8 );
		newScore->setTextColor( cocos2d::Color4B::BLACK );
		newScore->setPosition( background->getPosition().x,
			background->getPosition().y + background->getBoundingBox().size.height / 2 - newScore->getBoundingBox().size.height / 2 );

		_highscoreLayer->addChild( newScore );

		//edit highscore name box
		nameField = ui::TextField::create( "ABC", "fonts/Marker Felt.ttf", 30 );
		nameField->setScaleX( ( visibleSize.width / nameField->getBoundingBox().size.width ) * 1/6 );
		nameField->setScaleY( ( visibleSize.height / nameField->getBoundingBox().size.height ) * 1/8 );
		nameField->setName( "nameField" );
		nameField->setPosition( Vec2( background->getPosition().x,
			newScore->getPosition().y - newScore->getBoundingBox().size.height / 2 - nameField->getBoundingBox().size.height / 2 ) );
		nameField->setMaxLength( 3 );
		nameField->setTouchAreaEnabled( true );
		nameField->setTouchEnabled( true );
		nameField->setTouchSize( Size( 200, 200) );
		nameField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
			if(nameField->getStringLength() == 0 )
			{
				nameField->setString( "ABC" );
			}
            def->setStringForKey( NAME_SCORE, nameField->getString() );

		});

		_highscoreLayer->addChild( nameField );

		//adds done button
		auto donebutton_item = ui::Button::create( "donebutton1.png", "donebutton2.png" );
		donebutton_item->setScaleX( ( visibleSize.width / donebutton_item->getBoundingBox().size.width ) * 1/4 );
		donebutton_item->setScaleY( ( visibleSize.height / donebutton_item->getBoundingBox().size.height ) * 1/8 );
		donebutton_item->setPosition( Vec2( background->getPosition().x,
			background->getPosition().y - background->getBoundingBox().size.height / 2 + donebutton_item->getBoundingBox().size.height / 2 + 10 )  );
		donebutton_item->addTouchEventListener( CC_CALLBACK_1( GameLayer::doneButton, this ) );

		_highscoreLayer->addChild( donebutton_item );

		GameLayer::_gameScene->addChild( _highscoreLayer );

	}
}

void GameLayer::initGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getWinSize();

	//draw background
	cocos2d::Sprite* backgroundSprite = Sprite::create( "background.png" );
	backgroundSprite->setScaleX((winSize.width / backgroundSprite->getContentSize().width));
	backgroundSprite->setScaleY((winSize.height / backgroundSprite->getContentSize().height));
	backgroundSprite->setPosition( Vec2( origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 ) );
	_gameScene->addChild( backgroundSprite, -10 );

	//Create the player
	GameController::spawnPlayer();
	GameController::_player->setPosition( Point( origin.x + visibleSize.width / 6, origin.y + visibleSize.height / 2 ) );
	_gameScene->addChild( GameController::_player, 1 );

	//draw the score
	auto scoreText = Label::create( "Score:   ", "fonts/Marker Felt.ttf", 30);
	//scoreText->setScaleX( ( visibleSize.width / scoreText->getBoundingBox().size.width ) * 1/3 );
	//scoreText->setScaleY( ( visibleSize.height / scoreText->getBoundingBox().size.height ) * 1/8 );
	scoreText->setTextColor( cocos2d::Color4B::BLACK );
	scoreText->setPosition( Vec2( origin.x + scoreText->getBoundingBox().size.width / 2 + 5,
		origin.y + visibleSize.height - scoreText->getBoundingBox().size.height / 2 - 5) );
	scoreText->setName( "scoreText" );

	_gameScene->addChild( scoreText );

	auto scoreInt = Label::create( "000", "fonts/Marker Felt.ttf", 30);
	//scoreInt->setScaleX( ( visibleSize.width / scoreInt->getBoundingBox().size.width ) * 1/6 );
	//scoreInt->setScaleY( ( visibleSize.height / scoreInt->getBoundingBox().size.height ) * 1/8 );
	scoreInt->setTextColor( cocos2d::Color4B::BLACK );
	scoreInt->setPosition( Vec2( scoreText->getPosition().x + scoreText->getBoundingBox().size.width / 2 + scoreInt->getBoundingBox().size.width / 2 ,
		scoreText->getPosition().y ) );
	scoreInt->setName( "scoreInt" );

	_gameScene->addChild( scoreInt );
	
	//draw amount of arrows
	auto nrArrows = Label::create("Arrows: 4 / 4", "fonts/Marker Felt.ttf", 30 );
	//nrArrows->setScaleX( ( visibleSize.width / nrArrows->getBoundingBox().size.width ) * 1/6 );
	//nrArrows->setScaleY( ( visibleSize.height / nrArrows->getBoundingBox().size.height ) * 1/8 );
	nrArrows->setTextColor( cocos2d::Color4B::BLACK );
	nrArrows->setPosition( Vec2( scoreInt->getPosition().x + scoreInt->getBoundingBox().size.width / 2 + nrArrows->getBoundingBox().size.width / 2 + 20,
		origin.y + visibleSize.height - nrArrows->getBoundingBox().size.height / 2 - 5 ) );
	nrArrows->setName( "nrArrows" );

	_gameScene->addChild( nrArrows );
}

void GameLayer::retryGame( cocos2d::Ref* pSender )
{
	_gameScene->removeChildByName( "lostLayer" );
	_gameScene->removeChildByName( "scoreText" );
	_gameScene->removeChildByName( "scoreInt" );
	//_gameScene->removeChildByName( "nrArrows" );

	GameLayer::_dead = false;
	GameLayer::initGame();
}

void GameLayer::backToMenu( cocos2d::Ref* pSender )
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	if( audio->isBackgroundMusicPlaying() )
	{
		audio->stopBackgroundMusic();
	}

	_gameScene->removeAllChildrenWithCleanup( true );
	//_gameScene->removeChildByName( "lostLayer" );
	//_gameScene->removeChildByName( "scoreText" );
	//_gameScene->removeChildByName( "scoreInt" );
	//_gameScene->removeChildByName( "nrArrows" );

	GameLayer::_dead = false;
	auto scene = MainMenu::createScene();
	Director::getInstance()->pushScene( scene );
}

void GameLayer::doneButton(Ref* sender)
{
	GameLayer::_gameScene->removeChildByName( "highscoreLayer" );

	//set the new highscore and name
	Label* labelScore = (Label* )  _gameScene->getChildByName( "scoreInt" );
	auto def = CCUserDefault::sharedUserDefault();
	def->setIntegerForKey( HIGH_SCORE, stdReplacer::stoi( labelScore->getString() ) );

	_lostLayer->getChildByName( "menu" )->setVisible( true );
}

void GameLayer::rCast( Vec2 start, Vec2 end )
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect( "sniper.wav", false, 1.0f, 1.0f, 1.0f );

	Vec2 points[5];
    int num = 0;
    auto func = [&points, &num](PhysicsWorld& world,
        const PhysicsRayCastInfo& info, void* data)->bool
    {
        if (num < 5)
        {
            points[num++] = info.contact;
        }
        return true;
    };

	GameLayer::_gameScene->getPhysicsWorld()->rayCast( func, start, end, nullptr );

	for (int i = 0; i < num; ++i)
    {
		if( abs( GameController::_player->getPosition().x - points[i].x ) < GameController::_player->getBoundingBox().size.width )
		{
			GameLayer::playerDead();
		}
    }

}
