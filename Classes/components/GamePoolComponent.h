#ifndef __GAME_POOL_COMPONENT_H__
#define __GAME_POOL_COMPONENT_H__

#include "cocos2d.h"
#include "MyComponent.h"
#include <string>

//namespace ResourcePoolNS {
//	class ResourcePool;
//	class PoolVector;
//}

class ResourcePool;
class PoolVector;

class GamePoolComponent : public MComponent{
public:
	friend class ResourcePool;
	friend class PoolVector;

	GamePoolComponent(PoolVector *pool, size_t idx = 0, const std::string& key = "") : poolPtr(pool), idx(idx), key(key) {}
	virtual ~GamePoolComponent(){}

	std::string getKey() { return key; }
	size_t getIdx() { return idx; }
	PoolVector* getPool() { return poolPtr; }
	virtual bool isValid() const override { return !key.empty(); }
protected:
	size_t idx = 0;
	std::string key;
	PoolVector *poolPtr = nullptr; // weakRef -> ptr co the point de 1 invalid object
};

#endif // __GAME_POOL_COMPONENT_H__
