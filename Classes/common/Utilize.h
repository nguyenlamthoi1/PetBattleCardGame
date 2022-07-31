#ifndef __UTILIZE_H__
#define __UTILIZE_H__

#include "cocos2d.h"
#include "components/MyComponent.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

#define UTILIZE_NS_BEGIN namespace Utilize {
#define UTILIZE_NS_END }
#define UTILIZE_USE_NS using namespace Utilize

#define UTIL_STR_NS_BEGIN namespace mstr {
#define UTIL_STR_NS_END }

#define UTIL_VECTOR_BEGIN namespace mvector {
#define UTIL_VECTOR_END }

//#define UTIL_NODE_NS_BEGIN namespace node {
//#define UTIL_NODE_NS_END }


UTILIZE_NS_BEGIN

UTIL_STR_NS_BEGIN

bool has_suffix(const std::string &s, const std::string &suffix);

UTIL_STR_NS_END


cocos2d::Node* getChildByMap(cocos2d::Node* root, const std::string &childName, const std::string &parentName);
cocos2d::Node* getChildByMap(cocos2d::Node* root, const std::string &name);
cocos2d::Node* getChildByName(cocos2d::Node* root, const std::string &name);


UTIL_VECTOR_BEGIN

// vector
template <typename T>
bool removeVectorElement(std::vector<T>& vector, const T& element)
{
	auto it = std::find(vector.begin(), vector.end(), element);
	if (it != vector.end())
	{
		vector.erase(it);
		return true;
	}
	return false;
};

UTIL_VECTOR_END

UTILIZE_NS_END
#endif // __UTILIZE_H__