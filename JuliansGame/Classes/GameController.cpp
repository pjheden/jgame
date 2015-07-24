#include "GameController.h"
USING_NS_CC;

Vector<Bullet*> GameController::bullets;
Vector<EnemyCB*> GameController::enemies;

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
	
	Bullet* bullet = nullptr;
	switch( type )
	{
	case 1 : //indian
		{
		bullet = Arrow::create();
		bullet->setPosition( pos );
		bullet->setTarget( tar );
		
		break;
		} 
	case 2 : //cowboy
		{
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

		cbSprite->schedule(schedule_selector( EnemyCB::shoot ), 6.0f);

		break;

		
	}
	if ( cbSprite )
	{
		enemies.pushBack( cbSprite );
		return cbSprite;
	}
	return cbSprite;
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
		bullets.eraseObject( (Bullet*) node );
		break;
	}
	node->removeFromParentAndCleanup( true );
}

