#include "ComponentDictionary.h"
#include <new>

USING_NS_CC;
using namespace std;

ComponentDictionary* ComponentDictionary::create() {
	ComponentDictionary *ret = new (nothrow) ComponentDictionary();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	return ret;
}

bool ComponentDictionary::init() {
	return true;
}

ComponentDictionary::ComponentDictionary() {

}

ComponentDictionary::~ComponentDictionary() {

}

std::pair<ComponentDictionary::itrType, bool> ComponentDictionary::insert(const string &key, MComponent *compPtr) {
	if (key.empty())
	{
		CCLOG("ComponentDictionary::insert: empty key");
		return make_pair(map.end(), false);
	}
	if (!compPtr->isValid())
	{
		CCLOG("ComponentDictionary::insert: invalid component");
		return make_pair(map.end(), false);
	}

	return map.insert(make_pair(key, CompShPtr(compPtr)));
}

ComponentDictionary::size_type ComponentDictionary::erase(const std::string &key) {
	CCASSERT(!key.empty(), "ComponentDictionary::erase: Invalid Argument!");
	return map.erase(key);
}

ComponentDictionary::CompShPtr ComponentDictionary::at(const std::string &key) const {
	auto it = map.find(key);
	if (it == map.end())
		return shared_ptr<MComponent>(); // Return nullptr
	else
		return it->second;
}


