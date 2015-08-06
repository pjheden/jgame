#include "stdReplacer.h"


stdReplacer::stdReplacer(void)
{
}


stdReplacer::~stdReplacer(void)
{
}

std::string stdReplacer::to_string(int i)
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	std::ostringstream os;
	os << i;

	return os.str();
	#else
	return std::to_string(i);
	#endif
}

int stdReplacer::stoi(std::string s)
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return atoi(s.c_str());
	#else
	return std::stoi(s);
	#endif
}
