#include "ResourcePool.h"
#include "components/MyComponent.hpp"
#include "cocostudio/ActionTimeline/CSLoader.h"


#include <new.h>

using namespace cocos2d;
using namespace std;
MY_COMPONENT_USE_NS;

RESOURCE_POOL_NS_BEG

bool ResourcePool::addNode(Node *node, const string& fileKey) {
	if (!node || fileKey.empty())
		return false;
	node->retain();
	pool[fileKey].nodes.push_back(node); // * Key khong ton tai van tao 1 empty object, freeIdx khong can thay doi
	CCLOG("ResourcePool::addNode %s used/total: %d / %d", fileKey.c_str(), pool[fileKey].freeIdx, pool[fileKey].nodes.size());
	setComponent(node, COMPONENT_KEY::GAME_POOL, new GamePoolComponent(pool[key]->nodes.size() - 1, key));
	return true;
}

bool ResourcePool::removeNode(Node *node, const string& fileKey) {

}

cocos2d::Node* ResourcePool::loadNodeFromCsb(const std::string &fileKey)
{
	CCLOG("ResourcePool::loadNode %s", fileKey.c_str());
	cocos2d::Node* node = CSLoader::createNode(fileKey);
	return node;
}


Node* ResourcePool::tryGetNode(const std::string &fileKey) {
	auto it = pool.find(fileKey);

	if (it == pool.end() || it->second.nodes.empty() || it->second.freeIdx < it->second.nodes.size()) {
		// * Neu khong tim thay key
		// * Hoac tim thay key, nhung empty
		// * Hoac tim thay key, khong empty, nhung khong co free node
		auto newNode = loadNodeFromCsb(fileKey);
		if (!newNode)
			throw std::string("ResourcePool::tryGetNode:: can't load new node ") + fileKey;
		addNode(newNode, fileKey);
	}

	// *Tai day, pool phai dam bao co free node
	auto &pvec = pool[fileKey];
	return pvec.nodes[pvec.freeIdx++];
}


RESOURCE_POOL_NS_END

FWPool* FWPool::_instance = nullptr;

FWPool* FWPool::getInstance() {
	if (_instance == nullptr) {
		_instance = new (std::nothrow) FWPool();
	}
	return _instance;
}

FWPool::FWPool() {

}

FWPool::~FWPool() {
	CCLOG("FWPool::destructor called");
	for (auto& it : pool) {
		PoolVector* pv = it.second;
		delete pv;
	}
	pool.clear();
}




void FWPool::removeNodes(const std::string& key) {
	if (key.empty())
	{
		// remove all
		for (auto& it : pool)
			removeNodes(it.first);
		return;
	}

	auto it = pool.find(key);
	if (it == pool.end())
		return;

	PoolVector* pv = it->second;
	std::vector<cocos2d::Node*>& nodes = pv->nodes;
	cocos2d::Node* node = nullptr;
	for (size_t i = 0, nodesSize = nodes.size(); i < nodesSize; i++)
	{
		node = nodes[i];
		GameComponent::remove(node, COMPONENT_KEY::GAME_POOL);
		int count = node->getReferenceCount();
		if (node->getParent() != nullptr)
		{
			count--;
			node->removeFromParent();
		}
		while (count-- > 0)
			node->release();
	}
	nodes.clear();
	pv->freeIdx = 0;
}



bool FWPool::returnNode(cocos2d::Node* node) {
	GamePoolComponent* comp = GameComponent::get<GamePoolComponent*>(node, COMPONENT_KEY::GAME_POOL);
	if (!comp || comp->getKey().empty()) {
		node->removeFromParent();
		return false; // Khong co trong pool
	}

	PoolVector* pv = pool[comp->getKey()];
	if (comp->getIdx() >= pv->freeIdx) {
		node->removeFromParent();
		return false; // Da return vao pool roi
	}

	node->removeFromParent();

	// re-arrange pool
	if (pv->freeIdx - 1 > comp->getIdx()) {
		// swap with a not-free node
		cocos2d::Node* tmpNode = pv->nodes[pv->freeIdx - 1];
		GamePoolComponent* tmpComp = GameComponent::get<GamePoolComponent*>(tmpNode, COMPONENT_KEY::GAME_POOL);
		pv->nodes[pv->freeIdx - 1] = node;
		pv->nodes[comp->getIdx()] = tmpNode;
		tmpComp->setIdx(comp->getIdx());
		comp->setIdx(pv->freeIdx - 1);
	}
	pv->freeIdx--;

	//Reset node's attributes to default value
	node->setVisible(true);
	node->setOpacity(255);
	node->setScale(1);
	node->setRotation(0);
	node->setColor(cocos2d::Color3B::WHITE);

	return true;
}

bool FWPool::returnNodes(const std::string& key)
{
	if (key.empty())
	{
		// return all
		for (auto& it : pool)
			returnNodes(it.first);
		return true;
	}

	auto it = pool.find(key);
	if (it == pool.end())
		return false;

	std::vector<cocos2d::Node*> nodes = it->second->nodes;
	for (size_t i = 0; i < nodes.size(); i++)
		returnNode(nodes[i]);

	return true;
}

void FWPool::returnNodesFromParent(cocos2d::Node* parent, bool returnParent)
{
	// cleanup before returning
	cocos2d::ui::Widget* widget = dynamic_cast<cocos2d::ui::Widget*>(parent);
	if (widget)
	{
		//FWUiComponent* comp = Component::get<FWUiComponent*>(widget, COMPONENT_KEY::UI);
		//if (comp)
		//	FWUI->cleanupData(widget);
	}

	// return children
	//if (parent->getChildren().size() > 0)
	//{
	//	std::vector<cocos2d::Node*> children = FWUTILS->cloneVector<cocos2d::Node*>(parent->getChildren());
	//	for (size_t i = 0, size = children.size(); i < size; i++)
	//		returnNodesFromParent(children[i], true);
	//}

	//// return parent
	//if (returnParent)
	//{
	//	FWPoolComponent* comp = Component::get<FWPoolComponent*>(parent, COMPONENT_KEY::UI);
	//	if (comp)
	//		returnNode(parent);
	//}
}

