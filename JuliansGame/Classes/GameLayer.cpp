#include "GameLayer.h"
#include "GameController.h"

USING_NS_CC;
bool GameLayer::_dead = false;
Scene* GameLayer::_gameScene;

GameLayer::GameLayer(void){}
GameLayer::~GameLayer(void){}

cocos2d::Scene* GameLayer::createScene()
{
	_gameScene = Scene::createWithPhysics();
	_gameScene -> getPhysicsWorld() -> setGravity( Vect( 0, 0 ) );
    //_gameScene -> getPhysicsWorld() -> setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL ); 

	auto layer = GameLayer::create();
	layer -> setPhyschisWorld( _gameScene -> getPhysicsWorld() );


    _gameScene->addChild(layer);

    return _gameScene;
}

bool GameLayer::init()
{
	 if ( !Layer::init() ) return false;

	//Create the background

	 GameLayer::initGame();

	// listen for contact between objects
	auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( GameLayer::onContactBegin, this) ;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );

	//init the schedule for the game, main loop
    schedule( schedule_selector( GameLayer::update ) );

	return true;
}

void GameLayer::update(float dt)
{
	if(_dead)
	{
		return;
	}

	//Update bullets
	 GameLayer::bulletControl();

	//spawn enemies
	 EnemyCB* cb;
	 EnemyCB* worker;
	 if(GameController::enemies.size() < 5)
	 {
		cb = GameController::spawnEnemy( 2 ); //int 2=cowboy, 3=worker
		if( cb )
		{
		addChild( cb );
		}
		worker = GameController::spawnEnemy( 3 ); //int 2=cowboy, 3=worker
		if( worker )
		{
		addChild( worker );
		}
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
		CCLOG("Player shot, game lost");
		GameLayer::_dead = true;
		GameController::eraseAll();
		GameLayer::lostLayer();

	}
	//if Worker, Cowboy collide with LeftWall
	//remove later
	else if( ( 2 == a->getCollisionBitmask() && 5 == b->getCollisionBitmask() ) 
		|| ( 5 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask() ) )
	{
		//remove Worker / Cowboy
		if( 2 == a->getCollisionBitmask() )
		{
			a->getNode()->removeFromParent();
		}else{
			b->getNode()->removeFromParent();
		}
		//HelloWorld::lostMenu();
	}
	   
    return true;
}

void GameLayer::updateScore( int nr )
{
	Label* scoreLabel = (Label*) this->getChildByName( "scoreInt" );
	if(scoreLabel)
	{
		auto str = scoreLabel->getString();
		int tempScore = std::stoi( str );
		scoreLabel->setString( std::to_string( tempScore + nr ) );
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
	lostText->setPosition( Vec2( origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 ) );
	layer->addChild( lostText );

	auto retryText = MenuItemFont::create( "Retry?", this, menu_selector( GameLayer::retryGame ) );
	retryText->setPosition( Vec2( lostText->getPosition().x, lostText->getPosition().y - retryText->getContentSize().height ) );

	auto menu = Menu::create( retryText, NULL );
	menu->setPosition(Vec2::ZERO);
	layer->addChild( menu );

	//GameLayer::_gameScene->unscheduleUpdate();
	GameLayer::_gameScene->addChild( layer );
}

void GameLayer::initGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//Create the player
	GameController::spawnPlayer();
	GameController::_player->setPosition( Point( origin.x + visibleSize.width / 10, origin.y + visibleSize.height / 2 ) );
	addChild( GameController::_player );

	//draw the score
	auto scoreText = Label::create( "Score:   ", "fonts/Marker Felt.ttf", 30);
	scoreText->setPosition( Vec2( origin.x + scoreText->getContentSize().width / 2,
		origin.y + visibleSize.height - scoreText->getContentSize().height / 2 ) );
	scoreText->setName( "scoreText" );

	this->addChild( scoreText );

	auto scoreInt = Label::create( "000", "fonts/Marker Felt.ttf", 30);
	scoreInt->setPosition( Vec2( scoreText->getPosition().x + scoreText->getContentSize().width / 2 + scoreInt->getContentSize().width / 2,
		scoreText->getPosition().y ) );
	scoreInt->setName( "scoreInt" );

	this->addChild( scoreInt );
}

void GameLayer::retryGame( cocos2d::Ref* pSender )
{
	_gameScene->removeChildByName( "lostLayer" );

	GameLayer::_dead = false;
	GameLayer::initGame();
}
