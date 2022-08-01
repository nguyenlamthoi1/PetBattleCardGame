#include "common/Utilize.h"
#include "components/ChildMapComponent.h"

USING_NS_CC;
using namespace std;
MY_COMPONENT_USE_NS;


UTILIZE_NS_BEGIN

/*
	String
*/
UTIL_STR_NS_BEGIN
bool has_suffix(const std::string &s, const std::string &suffix)
{
   return s.size() >= s.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}
UTIL_STR_NS_END

/*
	Node 
*/

Node* getChildByMap(Node* root, const string &childName, const string &parentName) {
	auto comp = getComponent<ChildMapComponent>(root, COMPONENT_KEY::CHILD_MAP);
	if (!comp || !comp->isValid()) { // * Them childmap component cho node chua co
		comp = new ChildMapComponent(root);
		setComponent(root, COMPONENT_KEY::CHILD_MAP, comp);
	}
	return comp->getChild(childName, parentName);
}
Node* getChildByMap(Node* root, const string &childName) {
	auto comp = getComponent<ChildMapComponent>(root, COMPONENT_KEY::CHILD_MAP);
	if (!comp || !comp->isValid()) { // * Them childmap component cho node chua co
		comp = new ChildMapComponent(root);
		setComponent(root, COMPONENT_KEY::CHILD_MAP, comp);
	}
	return comp->getChild(childName);
}

Node* getChildByName(Node* root, const std::string &name) {
	if (root == nullptr) return nullptr;

	if (root->getName() == name)
		return root;

	cocos2d::Vector<Node*> &children = root->getChildren();
	Node* node = nullptr;
	for (const auto &child : children)
	{
		node = getChildByName(child, name);
		if (node)
			return node;
	}

	return node;
}

UTIL_NODE_BEGIN

Vec2 getLocalPos(Node *node, Node *localNode) {
	CCASSERT(node != nullptr, "Utilize::mnode::getLocalPos: node == nullptr");
	if (!localNode)
		return node->getPosition();
	auto wPos = node->getParent()->convertToWorldSpaceAR(node->getPosition());
	return localNode->convertToNodeSpaceAR(wPos);
}

cocos2d::Vec2 getWorldPos(cocos2d::Node *node) {
	CCASSERT(node != nullptr, "Utilize::mnode::getWorldPos: node == nullptr");
	return node->getParent()->convertToWorldSpaceAR(node->getPosition());
}


UTIL_NODE_END

UTILIZE_NS_END