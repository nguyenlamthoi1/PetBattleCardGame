#ifndef __RESOURCE_POOL_H__
#define __RESOURCE_POOL_H__

#include "cocos2d.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

//#define RESOURCE_POOL_NS_BEG namespace ResourcePoolNS {
//#define RESOURCE_POOL_NS_END }
//#define RESOURCE_POOL_USE_NS using namespace ResourcePoolNS

//RESOURCE_POOL_NS_BEG

class ResourcePool;

class PoolVector {
public:
	friend class ResourcePool;

	/* Hint: Luu y truoc khi return node:
	- Node phai co refCount = 1, (tuc la Node khong duoc co parent, hoac duoc retain o noi nao do)
	- Nen reset trang thai node truoc khi bo lai vao pool, vi node co the bi reuse
	*/
	static bool returnNode(cocos2d::Node *node); // Tra 1 node dang su dung ve pool

	//static void returnNodesFromParent(cocos2d::Node* parent, bool returnParent);
	//bool returnNodes(const std::string& key);

	PoolVector() = default;
	~PoolVector();
	void clear();
	void pushBack(cocos2d::Node *node, const std::string &fileKey);
	bool contains(cocos2d::Node *node) const;
	bool hasFreeNode() const;
private:
	std::vector<cocos2d::Node*> nodes;
	size_t freeIdx = 0;
};

class ResourcePool
{
public:
	static bool returnNode(cocos2d::Node *node) {
		return PoolVector::returnNode(node);
	}


	bool empty();
	
	/* Hint: function add 1 node bat ky vao pool voi fileKey
	- Return: ket qua add
	*/
	bool addNode(cocos2d::Node *node, const std::string &fileKey);

	/* Hint: function remove tat ca node co cung fileKey
	- Return: ket qua remove
	*/
	bool removeAllNodes(const std::string &fileKey);

	/* Hint: function remove tat ca node co trong pool
	*/
	void clear();
	
	/* Hint: function lay node duoc load tu file csb 
	- Co gang lay free csb node tu trong pool bang key
	- Neu nhu pool full hoac pool empty(khong co free node) thi load new node vao Pool, return new node
	- Khong lay node khong duoc load tu file csb tu ham nay
	*/
	cocos2d::Node* tryGetNodeCsb(const std::string &filekey);

	bool hasFreeNodeOfKey(const std::string &fileKey);

	/* Hint: function load node tu 1 file csb
	- Return: node duoc load
	*/
	cocos2d::Node* loadNodeFromCsb(const std::string &fileKey);


	~ResourcePool();
	ResourcePool();
private:
	std::unordered_map<std::string, PoolVector> pool;
};

//RESOURCE_POOL_NS_END

#endif // __RESOURCE_POOL_H__