#ifndef _ryCast_HPP_
#define _ryCast_HPP_

#include "cocos2d.h"

USING_NS_CC;

class ryCast
{
public:
	ryCast(void);
	~ryCast(void);
	static ryCast* create();

	// Getters
	Vec2 getStart() const;
	Vec2 getEnd() const;
	DrawNode* getNode() const;

	// Setters
	void setStart(Vec2 val);
	void setEnd(Vec2 val);
	void setNode(DrawNode* val);

private:
	Vec2 start_;
	Vec2 end_;
	DrawNode* node_;

};



#endif // _ryCast_HPP_