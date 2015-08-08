#ifndef _MainMenu_HPP_
#define _MainMenu_HPP_

#include "cocos2d.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

class MainMenu : public cocos2d::Layer
{
public:
	cocos2d::PhysicsWorld *sceneWorld;
	static cocos2d::Scene* createScene();
	CREATE_FUNC( MainMenu );
	virtual bool init();
	void startgame(Ref* sender);
	void settings(Ref* sender);
	void back(Ref* sender);
	void settingsSliderCallFunc(Ref* pSender, ui::Slider::EventType type);
	
private:
	MenuItemSprite* startgame_item;
	MenuItemSprite* settings_item;
	MenuItemSprite* back_item;
	void setPhyschisWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; }
};

#endif // _MainMenu_HPP_