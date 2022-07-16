#ifndef __UTILIZE_H__
#define __UTILIZE_H__

#include "cocos2d.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

#define UTILIZE_NS_BEGIN namespace Utilize {
#define UTILIZE_NS_END }
#define UTILIZE_USE_NS using namespace Utilize

#define UTIL_STR_NS_BEGIN namespace mstr {
#define UTIL_STR_NS_END }


UTILIZE_NS_BEGIN

UTIL_STR_NS_BEGIN

bool has_suffix(const std::string &s, const std::string &suffix) {
	return s.size() >= s.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

UTIL_STR_NS_END



UTILIZE_NS_END
#endif // __UTILIZE_H__