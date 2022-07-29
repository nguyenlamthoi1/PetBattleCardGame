#ifndef __MY_COMPONENT_H__
#define __MY_COMPONENT_H__

#include "cocos2d.h"
#include "ComponentDictionary.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

#define MY_COMPONENT_NS_BEG namespace MyComponentNS {
#define MY_COMPONENT_NS_END }
#define MY_COMPONENT_USE_NS using namespace MyComponentNS

namespace COMPONENT_KEY {
	//const std::string CONTENT = "CONTENT";
	//const std::string ONE_TOUCH_HANDLER = "ONE_TOUCH_HANDLER";
	//const std::string SPRITE_ANIMATOR = "SPRITE_ANIMATOR";
	const std::string GAME_POOL = "GAME_POOL";
	const std::string WIDGET_TOUCH = "WIDGET_TOUCH";
	const std::string CHILD_MAP = "CHILD_MAP";
}

class MComponent {
public:
	static const std::string GAME_POOL;

	MComponent() {};
	~MComponent() {}
	virtual bool isValid() const { return true; }; // Check Component hop le
};

MY_COMPONENT_NS_BEG

//template<typename T>
//T getComponent(cocos2d::Node *node, const std::string &compKey) {
//	cocos2d::Ref *obj = node->getUserObject();
//	if (!obj) // Object == nullptr
//		return nullptr;
//	cocos2d::Dictionary* dict = dynamic_cast<cocos2d::Dictionary*>(obj);
//	if (!dict) // Object != nullptr va Object khong phai Dictionary
//		return nullptr;
//
//	return dynamic_cast<T>(dict->objectForKey(compKey));
//}
template<typename T>
T* getComponent(cocos2d::Node *node, const std::string &compKey) {
	cocos2d::Ref *obj = node->getUserObject();
	if (!obj) // Object == nullptr
		return nullptr;
	auto dict = dynamic_cast<ComponentDictionary*>(obj);
	if (!dict) // Object != nullptr va Object khong phai Dictionary
		return nullptr;
	return dynamic_cast<T*>(dict->at(compKey).get());
}

std::shared_ptr<MComponent> getComponent(cocos2d::Node *node, const std::string &compKey);
void setComponent(cocos2d::Node *node, const std::string &key, MComponent *comp);
void removeComponent(cocos2d::Node *node, const std::string &compKey);

MY_COMPONENT_NS_END

#endif // __MY_COMPONENT_H__