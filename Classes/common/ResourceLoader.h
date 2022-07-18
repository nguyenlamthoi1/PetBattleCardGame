#ifndef __RESOURCE_LOADER_H_
#define __RESOURCE_LOADER_H_

#include "cocos2d.h"

#include <vector>
#include <memory>
#include <string>
#include <functional>

#define RESOURCE_LOADER_USE_MACRO_NS 1

#if defined(RESOURCE_LOADER_USE_MACRO_NS) && RESOURCE_LOADER_USE_MACRO_NS > 0
	#define RESOURCE_LOADER_NS_BEG namespace ResourceLoaderNS {
	#define RESOURCE_LOADER_NS_END }
	#define RESOURCE_LOADER_USE_NS using namespace ResourceLoaderNS
/*
	- RESOURCE_LOADER_DEBUG = 0 -> khong log khi run code
	- RESOURCE_LOADER_DEBUG > 0 -> log khi run code
*/
	#define RESOURCE_LOADER_DEBUG 1 

#else
	#define RESOURCE_LOADER_NS_BEG 
	#define RESOURCE_LOADER_NS_END 
	#define RESOURCE_LOADER_USE_NS
	#define RESOURCE_LOADER_DEBUG 1 
#endif

class GameManager;

RESOURCE_LOADER_NS_BEG

using LoadingFunc = std::function<bool()>;

class LoadedObject {
public:
	friend class ResourceLoader;
	LoadedObject(const std::string& name, LoadingFunc lfunc);
	void load();
private:
	LoadedObject() = default;
	std::string fname;
	LoadingFunc func;
	bool succeed = false;
};

class ResourceLoader {
public:
	static const std::string LOADER_SCHEDULER;
	using size_t = std::vector<LoadedObject>::size_type;

	friend class GameManager; // Class GameManager quan ly ResourceLoader object
	void startLoading();

	/* Hint: Ham dung de load cac sprite frame tu file plist*/
	void addPlistFile(const std::string &file);
	void addLoadedObj(const LoadedObject &obj);
	
	size_t stepCount() const { return loadedObjVec.size(); }

	~ResourceLoader(); // Dtor
private:
	void updateLoading(float);

	ResourceLoader();
	std::vector<LoadedObject> loadedObjVec;
	bool isLoading = false;
	size_t curStep = -1;
	unsigned int loaded = 0;
	unsigned int total = 0;
	//bool isPaused; // * Chi co y nghia khi isLoading == true
};

RESOURCE_LOADER_NS_END
#endif __RESOURCE_LOADER_H_