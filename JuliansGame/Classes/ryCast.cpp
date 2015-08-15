#include "ryCast.h"


ryCast::ryCast(void)
{
}


ryCast::~ryCast(void)
{
}

ryCast* ryCast::create()
{
	ryCast* rc = new ryCast();
	if( rc )
	{
		return rc;
	}
	
	CC_SAFE_DELETE(rc);
	return NULL;
}


void ryCast::setStart( Vec2 val )
{
	ryCast::start_ = val;
}

Vec2 ryCast::getStart() const
{
	return ryCast::start_;
}

void ryCast::setEnd( Vec2 val )
{
	ryCast::end_ = val;
}


Vec2 ryCast::getEnd() const
{
	return ryCast::end_;
}

void ryCast::setNode( DrawNode* val )
{
	ryCast::node_ = val;
}


DrawNode* ryCast::getNode() const
{
	return ryCast::node_;
}