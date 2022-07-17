#include "ResourcePool.h"
#include "components/GamePoolComponent.h"
#include "cocostudio/ActionTimeline/CSLoader.h"


#include <new.h>

using namespace cocos2d;
using namespace std;
MY_COMPONENT_USE_NS;

//RESOURCE_POOL_NS_BEG

PoolVector::~PoolVector() {
	clear();
}

void PoolVector::clear() {
	for (const auto& node : nodes) {
		removeComponent(node, COMPONENT_KEY::GAME_POOL);
		node->release();
	}
}

bool PoolVector::hasFreeNode() const {
	return freeIdx < nodes.size();
}


bool PoolVector::contains(Node *node) const
{
	for (const auto& nodeEle : nodes)
	{
		if (nodeEle == node)
			return true;
	}
	return false;
}

void PoolVector::pushBack(Node *node, const string &fileKey) {
	CCASSERT(node != nullptr, "PoolVector::pushBack: node is NULL");
	CCASSERT(!fileKey.empty(), "PoolVector::pushBack: fileKey is empty");
	node->retain();
	setComponent(node, COMPONENT_KEY::GAME_POOL, new GamePoolComponent(this, nodes.size() - 1, fileKey));
	nodes.push_back(node);
	CCLOG("PoolVector::pushBack %s used/total: %d / %d", fileKey.c_str(), freeIdx, nodes.size());
}

bool PoolVector::returnNode(Node *node) {
	auto comp = getComponent<GamePoolComponent>(node, COMPONENT_KEY::GAME_POOL);

	if (!comp || !comp->isValid())
		return false;

	// * Node phai co refCount = 1
	CCASSERT(node->getReferenceCount() == 1, "PoolVector::returnNode: node has refCount != 1");

	auto pool = comp->getPool();
	auto idxInPool = comp->getIdx();
	if (idxInPool >= pool->freeIdx) // Node da return trong pool
		return true;

	// re-arrange pool
	if (pool->freeIdx - 1 > idxInPool) {
		// swap voi  last non-free node co idx = freeIdx - 1
		Node* tmpNode = pool->nodes[pool->freeIdx - 1];
		auto tmpComp = getComponent<GamePoolComponent>(node, COMPONENT_KEY::GAME_POOL);
		pool->nodes[pool->freeIdx - 1] = node;
		pool->nodes[comp->idx] = tmpNode;
		tmpComp->idx = comp->idx;
		comp->idx = pool->freeIdx - 1;
	}
	pool->freeIdx--;

	//Reset node's attributes to default value
	//node->setVisible(true);
	//node->setOpacity(255);
	//node->setScale(1);
	//node->setRotation(0);
	//node->setColor(cocos2d::Color3B::WHITE);

	return true;
}


ResourcePool::~ResourcePool() {
	CCLOG("ResourcePool:dtor called");
}

bool ResourcePool::empty() {
	return pool.empty();
}

bool ResourcePool::addNode(Node *node, const string& fileKey) {
	if (!node || fileKey.empty())
		return false;
	pool[fileKey].pushBack(node, fileKey); // * Key khong ton tai van tao 1 empty object, freeIdx khong can thay doi
	return true;
}

void ResourcePool::clear() {
	pool.clear();
}

bool ResourcePool::removeAllNodes(const string& fileKey) {
	if (fileKey.empty()) // Khong pass parameter fileKey -> khong xoa
		return false;
	

	auto it = pool.find(fileKey);
	if (it == pool.end())
		return false; // Khong tim thay pool vs key

	it->second.clear();
	return 0;
}

cocos2d::Node* ResourcePool::loadNodeFromCsb(const std::string &fileKey)
{
	CCLOG("ResourcePool::loadNode %s", fileKey.c_str());
	cocos2d::Node* node = CSLoader::createNode(fileKey);
	return node;
}

Node* ResourcePool::tryGetNodeCsb(const std::string &fileKey) {
	auto it = pool.find(fileKey);

	//if (it == pool.end() || it->second.nodes.empty() || it->second.freeIdx < it->second.nodes.size()) {
	if(!hasFreeNodeOfKey(fileKey)){
	// * Neu khong tim thay key
		// * Hoac tim thay key, nhung empty
		// * Hoac tim thay key, khong empty, nhung khong co free node
		auto newNode = loadNodeFromCsb(fileKey);
		if (newNode) {
			pool[fileKey].pushBack(newNode, fileKey); // * Key khong ton tai van tao 1 empty object, freeIdx khong can thay doi
			++(pool[fileKey].freeIdx);
			return newNode;
		}
		else
			return nullptr; // Khong load duoc node -> return NULL
	}

	// *Tai day, pool phai dam bao co free node
	auto &pvec = pool[fileKey];
	return pvec.nodes[pvec.freeIdx++]; // Tang freeIdx len
}

bool ResourcePool::hasFreeNodeOfKey(const std::string &fileKey) {
	auto it = pool.find(fileKey);
	return it != pool.end() && it->second.hasFreeNode();
}


//RESOURCE_POOL_NS_END
