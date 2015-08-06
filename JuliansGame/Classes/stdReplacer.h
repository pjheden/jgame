#ifndef _stdReplacer_HPP_
#define _stdReplacer_HPP_

#include "cocos2d.h"

USING_NS_CC;

class stdReplacer
{
public:
	stdReplacer(void);
	~stdReplacer(void);

	static std::string to_string(int i);
	static int stoi(std::string s );
};



#endif // _stdReplacer_HPP_