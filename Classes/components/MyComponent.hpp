//#ifndef __MY_COMPONENT_H__
//#define __MY_COMPONENT_H__
//
//#include "cocos2d.h"
//#include "ComponentDictionary.h"
//
//#include <vector>
//#include <memory>
//#include <string>
//#include <functional>
//
//#define MY_COMPONENT_NS_BEG namespace MyComponentNS {
//#define MY_COMPONENT_NS_END }
//#define MY_COMPONENT_USE_NS using namespace MyComponentNS
//
//class ComponentDictionary; // * Must declare this class;
//
//namespace COMPONENT_KEY {
//	const std::string CONTENT = "CONTENT";
//	const std::string ONE_TOUCH_HANDLER = "ONE_TOUCH_HANDLER";
//	const std::string CHILD_MAP = "CHILD_MAP";
//	const std::string GAME_POOL = "GAME_POOL";
//	const std::string SPRITE_ANIMATOR = "SPRITE_ANIMATOR";
//}
//
//class MComponent {
//public:
//	static const std::string GAME_POOL;
//
//	MComponent() {};
//	~MComponent() {}
//	virtual bool isValid() const  { return true; } ; // Check Component hop le
//};
//
//MY_COMPONENT_NS_BEG
//
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
//
//void setComponent(cocos2d::Node *node, const std::string &key, MComponent *comp) {
//	cocos2d::Ref *obj = node->getUserObject();
//	ComponentDictionary *dict = nullptr;
//	if (!obj) {
//		dict = ComponentDictionary::create();
//		node->setUserObject(dict);
//	}
//	//else {
//	//	dict = dynamic_cast<ComponentDictionary*>(obj);
//	//	CCASSERT(dict == nullptr, "setComponent: node already has attached another component");
//	//}
//
//	//dict->erase(key); // Xoa old component co trung key
//	//dict->insert(key, comp); // Insert component moi
//}
//
//void removeComponent(cocos2d::Node *node, const std::string &compKey) {
//	cocos2d::Ref* obj = node->getUserObject();
//	if (!obj) // Object == nullptr
//		return;
//
//	cocos2d::Dictionary* dict = dynamic_cast<cocos2d::Dictionary*>(obj);
//	if (!dict) // Object != nullptr va Object khong phai Dictionary
//		return;
//	dict->removeObjectForKey(compKey);
//}
//
//MY_COMPONENT_NS_END
//
//#endif // __MY_COMPONENT_H__