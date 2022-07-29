#ifndef __CHILD_MAP_COMPONENT_H__
#define __CHILD_MAP_COMPONENT_H__

#include "cocos2d.h"
#include "MyComponent.h"
#include <string>

class ChildMapComponent : public MComponent{
public:
	ChildMapComponent(cocos2d::Node *n) : node(n) {
		for (const auto &child : n->getChildren()) {
			map.insert({child->getName(), child}); // 1 element is {name, node}
		}
	}
	virtual ~ChildMapComponent() = default;
	virtual bool isValid() const override { return true; }
	cocos2d::Node* getChild(const std::string &name) {
		auto iter = map.find(name);
		return iter == map.cend() ? nullptr : iter->second;
	}
	cocos2d::Node* getChild(const std::string &name, const std::string &parentName) {
		for (auto beg = map.lower_bound(name), end = map.upper_bound(name); beg != end; ++beg) {
			auto &node = beg->second;
			if (node && node->getName() == parentName)
				return node;
		}
		return nullptr;
	}
protected:
	cocos2d::Node *node = nullptr;
	std::unordered_multimap<std::string, cocos2d::Node*> map;
};

#endif // __GAME_POOL_COMPONENT_H__
