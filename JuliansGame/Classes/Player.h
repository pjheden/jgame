#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "cocos2d.h"

USING_NS_CC;

class Player : public cocos2d::Sprite
{
    public:

		Animate * idleAnimate;
		Animate * moveAnimate;
		Animate * shootAnimate;
		bool	moving;
		int dir;
		int nrOfArrows;

        Player();
        ~Player();
        static Player* create();

		PhysicsBody* getBody();

        void initOptions();

        void addEvents();
        void touchEvent( cocos2d::Touch* touch, cocos2d::Vec2 _p );
		void initAnimations();

		void move( cocos2d::Vec2 direction );

		void idle();

		void shoot();

		bool onTouchBegan( Touch *touch, Event *unused_event );
		void onTouchMoved( Touch *touch, Event *unused_event);
		void onTouchEnded( Touch *touch, Event *unused_event );

    private:

};

#endif // _PLAYER_HPP_