#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "cocos2d.h"

USING_NS_CC;

class Player : public cocos2d::Sprite
{
    public:

		Animate * idleAnimate;
		Animate * moveAnimate;

        Player();
        ~Player();
        static Player* create();

        void initOptions();

        void addEvents();
        void touchEvent( cocos2d::Touch* touch, cocos2d::Vec2 _p );
		void initAnimations();

    private:

};

#endif // _PLAYER_HPP_