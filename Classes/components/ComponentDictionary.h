#ifndef __COMPONENT_DICTIONARY_H__
#define __COMPONENT_DICTIONARY_H__

#include "cocos2d.h"
#include "MyComponent.h"
#include <unordered_map>
#include <string>
#include <utility>

class MComponent;

class ComponentDictionary : public cocos2d::Ref { 
public:
	using CompShPtr = std::shared_ptr<MComponent>;
	using size_type = std::unordered_map<std::string, CompShPtr>::size_type;
	using strK = std::unordered_map<std::string, CompShPtr>::key_type; // kieu std::string
	using compV = std::unordered_map<std::string, CompShPtr>::mapped_type; // kieu MComponent
	using pairEle = std::unordered_map<std::string, CompShPtr>::value_type; // kieu pair<const std::string, MComponent>
	using itrType = std::unordered_map<std::string, CompShPtr>::iterator; // Kieu cua iterator
	using constItrType = std::unordered_map<std::string, CompShPtr>::const_iterator; // Kieu cua const iterator
	
	static ComponentDictionary* create();

	ComponentDictionary();
	virtual ~ComponentDictionary();

	CompShPtr at(const std::string &key) const;

	std::pair<itrType, bool> insert(const std::string &key, MComponent *compPtr); // *comPtr phai point den 1 dynamic memoory
	size_type erase(const std::string &key);

private:
	std::unordered_map<std::string, CompShPtr> map;
	bool init(); // * luon return true
};

#endif // __COMPONENT_DICTIONARY_H__
