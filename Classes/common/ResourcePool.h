#ifndef __RESOURCE_POOL_H__
#define __RESOURCE_POOL_H__

#include "cocos2d.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

#define RESOURCE_POOL_NS_BEG namespace ResourcePoolNS {
#define RESOURCE_POOL_NS_END }
#define RESOURCE_POOL_USE_NS using namespace ResourcePoolNS

RESOURCE_POOL_NS_BEG

class ResourcePool;

class PoolVector {
public:
	friend class ResourcePool;
private:
	std::vector<cocos2d::Node*> nodes;
	size_t freeIdx = 0;
	PoolVector() = default;
	~PoolVector() {
		CCLOG("PoolVector::dtor called");
	}
};

class ResourcePool
{
public:
	bool addNode(cocos2d::Node* node, const std::string &fileKey);
	bool removeNode(cocos2d::Node* node, const std::string &fileKey);
	
	void returnNode(cocos2d::Node* node, const std::string &fileKey);


	/* Hint: tryGetNode co gang lay free node tu trong pool bang key
	- Neu nhu pool full hoac pool empty(khong co free node) thi load new node vao Pool, return new node
	*/
	cocos2d::Node* tryGetNode(const std::string &filekey);
	cocos2d::Node* loadNodeFromCsb(const std::string &fileKey);

	~ResourcePool();
private:
	ResourcePool();

	std::unordered_map<std::string, PoolVector> pool;
	
	void removeNodes(const std::string& key = "");


	template<typename T>
	T getNode(const std::string& key) {
		cocos2d::Node* node = tryGetNode(key);
		if (!node)
			return nullptr;
		return static_cast<T>(node);
	}

	bool returnNode(cocos2d::Node* node);
	void returnNodesFromParent(cocos2d::Node* parent, bool returnParent);
	bool returnNodes(const std::string& key);
};

RESOURCE_POOL_NS_END

#endif // __RESOURCE_POOL_H__