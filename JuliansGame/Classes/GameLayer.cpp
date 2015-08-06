#include "GameLayer.h"
#include "GameController.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "stdReplacer.h"

USING_NS_CC;
const char* HIGH_SCORE="key1";

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

	//Update bullets
	 GameLayer::bulletControl();
	
	 //update arrow counter
	 GameLayer::updateArrows();


	//spawn enemies
	 EnemyCB* cb;
	 //EnemyCB* worker;
	 if(GameController::enemies.size() < 5)
	 {
		cb = GameController::spawnEnemy( 2 ); //int 2=cowboy, 3=worker
		if( cb )
		{
			addChild( cb );
		}

		//worker = GameController::spawnEnemy( 3 ); //int 2=cowboy, 3=worker
		//if( worker )
		//{
		//	addChild( worker );
		//}
	 }

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
		GameLayer::_dead = true;
		GameController::eraseAll();
		GameLayer::lostLayer();
	}
	////if arrow hits right wall
	//else if( ( 6 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask() ) 
	//	|| ( 3 == a->getCollisionBitmask() && 6 == b->getCollisionBitmask() ) )
	//{
	//	if( a->getCollisionBitmask()==3 )
	//	{
	//		GameController::erase( a->getNode() );
	//	}
	//	else
	//	{
	//		GameController::erase( b->getNode() );
	//	}

	//}
	////if bullets hits left wall
	//else if( ( 4 == a->getCollisionBitmask() && 5 == b->getCollisionBitmask() ) 
	//	|| ( 5 == a->getCollisionBitmask() && 4 == b->getCollisionBitmask() ) )
	//{
	//	if( a->getCollisionBitmask()==4 )
	//	{
	//		GameController::erase( a->getNode() );
	//	}
	//	else
	//	{
	//		GameController::erase( b->getNode() );
	//	}
	//}


    return true;
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

	auto layer = Layer::create();
	layer->setName( "lostLayer" );

	auto lostText = Label::create( "Game Over", "fonts/Marker Felt.ttf", 30);
	lostText->setPosition( Vec2( origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + lostText->getContentSize().height) );
	layer->addChild( lostText );

	auto retryText = MenuItemFont::create( "Retry?", this, menu_selector( GameLayer::retryGame ) );
	retryText->setPosition( Vec2( lostText->getPosition().x, lostText->getPosition().y - 2 * retryText->getContentSize().height ) );

	auto menuText = MenuItemFont::create( "Menu", this, menu_selector( GameLayer::backToMenu ) );
	menuText->setPosition( Vec2( retryText->getPosition().x, retryText->getPosition().y - menuText->getContentSize().height ) );

	auto menu = Menu::create( retryText, menuText, NULL );
	menu->setPosition(Vec2::ZERO);
	layer->addChild( menu );

	

	//draws the Highscore 
	auto def = CCUserDefault::sharedUserDefault();
	auto highScoreKey = def->getIntegerForKey( HIGH_SCORE );
	char score[10];
	sprintf( score, "%d", highScoreKey );

	std::string tempString( "Highscore: ");
	//checks if its a highscore
	auto playerScore = (Label* )  _gameScene->getChildByName( "scoreInt" );
	if( stdReplacer::stoi(playerScore->getString() ) > highScoreKey )
	{
		tempString += playerScore->getString();
		def->setIntegerForKey(HIGH_SCORE, stdReplacer::stoi(playerScore->getString() ));
		def->flush();
	} else {
		tempString += score;
	}

	auto highScore = Label::create( tempString , "fonts/Marker Felt.ttf", 30 );
	highScore->setPosition( Vec2( origin.x + visibleSize.width / 2, origin.y + visibleSize.height - highScore->getContentSize().height / 2 ) );
	layer->addChild( highScore );

	//GameLayer::_gameScene->unscheduleUpdate();
	GameLayer::_gameScene->addChild( layer );
}

void GameLayer::initGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//draw background
	cocos2d::Sprite* backgroundSprite = Sprite::create( "background.png" );
	backgroundSprite->setPosition( Vec2( origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 ) );
	backgroundSprite->setLocalZOrder( -10 );
	_gameScene->addChild( backgroundSprite );

	//Create the player
	GameController::spawnPlayer();
	GameController::_player->setPosition( Point( origin.x + visibleSize.width / 6, origin.y + visibleSize.height / 2 ) );
	_gameScene->addChild( GameController::_player );

	//draw the score
	auto scoreText = Label::create( "Score:   ", "fonts/Marker Felt.ttf", 30);
	scoreText->setPosition( Vec2( origin.x + scoreText->getContentSize().width / 2,
		origin.y + visibleSize.height - scoreText->getContentSize().height / 2 ) );
	scoreText->setName( "scoreText" );

	_gameScene->addChild( scoreText );

	auto scoreInt = Label::create( "000", "fonts/Marker Felt.ttf", 30);
	scoreInt->setPosition( Vec2( scoreText->getPosition().x + scoreText->getContentSize().width / 2 + scoreInt->getContentSize().width / 2,
		scoreText->getPosition().y ) );
	scoreInt->setName( "scoreInt" );

	_gameScene->addChild( scoreInt );
	
	//draw amount of arrows
	auto nrArrows = Label::create("Arrows: 4 / 4", "fonts/Marker Felt.ttf", 30 );
	nrArrows->setPosition( Vec2( origin.x + visibleSize.width - nrArrows->getContentSize().width / 2,
		origin.y + visibleSize.height - nrArrows->getContentSize().height / 2 ) );
	nrArrows->setName( "nrArrows" );

	_gameScene->addChild( nrArrows );
}

void GameLayer::retryGame( cocos2d::Ref* pSender )
{
	_gameScene->removeChildByName( "lostLayer" );
	_gameScene->removeChildByName( "scoreText" );
	_gameScene->removeChildByName( "scoreInt" );
	_gameScene->removeChildByName( "nrArrows" );

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

	auto scene = HelloWorld::createScene();
	Director::getInstance()->pushScene( scene );
}
