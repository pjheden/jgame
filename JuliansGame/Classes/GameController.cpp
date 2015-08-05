#include "GameController.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

USING_NS_CC;

Vector<Bullet*> GameController::bullets;
Vector<EnemyCB*> GameController::enemies;
Player* GameController::_player;

GameController::GameController(void)
{
}
GameController::~GameController(void)
{
}

bool GameController::init()
{
	return true;
};

Bullet* GameController::spawnBullet( int type, Vec2 pos, Vec2 tar)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	Bullet* bullet = nullptr;
	switch( type )
	{
	case 1 : //indian
		{
			if (_player->nrOfArrows >= 0)
			{
				audio->playEffect( "arrow.wav", false, 1.0f, 1.0f, 1.0f );
				bullet = Arrow::create();
				bullet->setPosition( pos );
				bullet->setTarget( tar );
				_player->nrOfArrows -= 1;
			}
		break;
		} 
	case 2 : //cowboy
		{
		audio->playEffect( "gunshot.wav", false, 1.0f, 1.0f, 1.0f );
		bullet = Bullet::create();
		bullet->setPosition( pos );
		bullet->setTarget( tar );
		/*auto bulletSpeed = 300.0f;
		auto distance = pos.x - origin.x;
		auto move_action = MoveTo::create( distance / bulletSpeed, Vec2( origin.x, pos.y ) );*/
		break;
		}
	}

	if( bullet ) 
    {
        bullets.pushBack( bullet );
        bullet->setPosition( pos );
        return bullet;
    }
    return bullet;

};

EnemyCB* GameController::spawnEnemy( int type )
{
	EnemyCB* cbSprite = nullptr;
	switch( type )
	{
	case 3:
		cbSprite= Worker::create();
		cbSprite->setPhysicsBody( cbSprite->getBody() );
		break;
	case 2:
		cbSprite= EnemyCB::create();
		cbSprite->setPhysicsBody( cbSprite->getBody() );

		cbSprite->schedule(schedule_selector( EnemyCB::shoot ), RandomHelper::random_int( 4.0f, 6.0f ) );

		break;

		
	}
	if ( cbSprite )
	{
		enemies.pushBack( cbSprite );
		return cbSprite;
	}
	return cbSprite;
}

Player* GameController::spawnPlayer()
{
	GameController::_player = Player::create();
	GameController::_player->setPhysicsBody ( GameController::_player->getBody() );

	return _player;
}

void GameController::erase( Node* node )
{
	auto type = node->getPhysicsBody()->getCollisionBitmask();
	switch ( type )
	{
	case 1://player
		break;
	case 2: //enemy
		enemies.eraseObject( (EnemyCB*) node );
		break;
	case 3: //projectile
		//GameController::_player->nrOfArrows -= 1;
		bullets.eraseObject( (Bullet*) node );
		break;
	}
	node->removeFromParentAndCleanup( true );
}

void GameController::eraseAll()
{
	//remove player & touch events
	GameController::_player->removeFromParentAndCleanup( true );
//	GameController::_player->getEventDispatcher()->removeAllEventListeners();

	//remove all bullets
	while( !bullets.empty() )
	{
		bullets.at( 0 )->removeFromParentAndCleanup( true );
		bullets.erase( 0 );
	}

	//remove all enemies
	while( !enemies.empty() )
	{
		enemies.at( 0 )->removeFromParentAndCleanup( true );
		enemies.erase( 0 );
	}
}