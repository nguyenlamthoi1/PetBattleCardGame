#include "MyComponent.h"
//#include "ComponentDictionary.h"

USING_NS_CC;
using namespace std;

MY_COMPONENT_NS_BEG

shared_ptr<MComponent> getShComp(Node *node, const string &compKey) {
	cocos2d::Ref *obj = node->getUserObject();
	if (!obj) // Object == nullptr
		return nullptr;
	auto dict = dynamic_cast<ComponentDictionary*>(obj);
	if (!dict) // Object != nullptr va Object khong phai Dictionary
		return nullptr;
	auto a = dict->at(compKey);
	return dict->at(compKey);
}

MComponent* getComp(cocos2d::Node *node, const std::string &compKey) {
	cocos2d::Ref *obj = node->getUserObject();
	if (!obj) // Object == nullptr
		return nullptr;
	auto dict = dynamic_cast<ComponentDictionary*>(obj);
	if (!dict) // Object != nullptr va Object khong phai Dictionary
		return nullptr;
	return dict->at(compKey).get();
}

void setComponent(Node *node, const string &key, MComponent *comp) {
	CCASSERT(node != nullptr, "setComponent: node is NULL");
	cocos2d::Ref *obj = node->getUserObject();
	ComponentDictionary *dict = nullptr;
	if (!obj) {
		dict = ComponentDictionary::create();
		node->setUserObject(dict);
	}
	else {
		dict = dynamic_cast<ComponentDictionary*>(obj);
		CCASSERT(dict != nullptr, "setComponent: node already has attached another component");
	}

	dict->erase(key); // Xoa old component co trung key
	dict->insert(key, comp); // Insert component moi
}

void removeComponent(Node *node, const string &compKey) {
	CCASSERT(node != nullptr, "setComponent: node is NULL");
	cocos2d::Ref* obj = node->getUserObject();
	if (!obj) // Object == nullptr
		return;

	auto dict = dynamic_cast<ComponentDictionary*>(obj);
	if (!dict) // Object != nullptr va Object khong phai Dictionary
		return;
	dict->erase(compKey);
}

void setComp(cocos2d::Node *node, const std::string &key, MComponent *comp) {
	CCASSERT(node != nullptr, "setComponent: node is NULL");
	cocos2d::Ref *obj = node->getUserObject();
	ComponentDictionary *dict = nullptr;
	if (!obj) {
		dict = ComponentDictionary::create();
		node->setUserObject(dict);
	}
	else {
		dict = dynamic_cast<ComponentDictionary*>(obj);
		CCASSERT(dict != nullptr, "setComponent: node already has attached another component");
	}

	dict->erase(key); // Xoa old component co trung key
	dict->insert(key, comp); // Insert component moi
}

void removeComp(cocos2d::Node *node, const std::string &compKey) {
	CCASSERT(node != nullptr, "setComponent: node is NULL");
	cocos2d::Ref* obj = node->getUserObject();
	if (!obj) // Object == nullptr
		return;

	auto dict = dynamic_cast<ComponentDictionary*>(obj);
	if (!dict) // Object != nullptr va Object khong phai Dictionary
		return;
	dict->erase(compKey);
}

MY_COMPONENT_NS_END