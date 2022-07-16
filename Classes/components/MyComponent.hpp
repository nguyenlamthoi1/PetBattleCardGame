#ifndef __MY_COMPONENT_H__
#define __MY_COMPONENT_H__

#include "cocos2d.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

#define MY_COMPONENT_NS_BEG namespace MyComponentNS {
#define MY_COMPONENT_NS_END }
#define MY_COMPONENT_USE_NS using namespace MyComponentNS

MY_COMPONENT_NS_BEG

template<typename T>
T getComponent(cocos2d::Node *node, const std::string &compKey) {
	cocos2d::Ref *obj = node->getUserObject();
	if (!obj) // Object == nullptr
		return nullptr;
	cocos2d::Dictionary* dict = dynamic_cast<cocos2d::Dictionary*>(obj);
	if (!dict) // Object != nullptr va Object khong phai Dictionary
		return nullptr;

	return dynamic_cast<T>(dict->objectForKey(compKey));
}

void setComponent(cocos2d::Node *node, const std::string &key, cocos2d::Ref *comp) {
	cocos2d::Ref *obj = node->getUserObject();
	cocos2d::Dictionary* dict = nullptr;
	if (!obj) {
		dict = cocos2d::Dictionary::create();
		node->setUserObject(dict);
	}
	else {
		dict = dynamic_cast<cocos2d::Dictionary*>(obj);
		CCASSERT(dict == nullptr, "setComponent: node already has attached another component");
	}

	dict->removeObjectForKey(key); // Xoa old component co trung key
	dict->setObject(comp, key);
	comp->autorelease();
}

void removeComponent(cocos2d::Node *node, const std::string &compKey) {
	cocos2d::Ref* obj = node->getUserObject();
	if (!obj) // Object == nullptr
		return;

	cocos2d::Dictionary* dict = dynamic_cast<cocos2d::Dictionary*>(obj);
	if (!dict) // Object != nullptr va Object khong phai Dictionary
		return;
	dict->removeObjectForKey(compKey);
}

MY_COMPONENT_NS_END

#endif // __MY_COMPONENT_H__