#include "ResourceLoader.h"
#include "define/CommonDefine.h"

#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIHelper.h"

#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"


USING_NS_CC;
using namespace std;

//namespace ResourceLoaderNS {
RESOURCE_LOADER_NS_BEG
//--Class LoadedObject--beg--
void LoadedObject::load() {
	succeed = func();
}
LoadedObject::LoadedObject(const string &name, LoadingFunc ldfunc) :
	fname(name),
	func(ldfunc)
{

}
//--Class LoadedObject--end--

//--Class ResourceLoader--beg--

const string ResourceLoader::LOADER_SCHEDULER = "ResourceLoader::updateLoading";
ResourceLoader::ResourceLoader() {}
ResourceLoader::~ResourceLoader() {
	CCLOG("ResourceLoader::dtor called");

	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->unschedule(LOADER_SCHEDULER, this); // Dung cap nhat loading
}

void ResourceLoader::startLoading() {
	CCLOG("ResourceLoader::startLoading called");
	if (isLoading)
		throw("start loading on a running ResourceLoader!");

	if (!loadedObjVec.empty()) {
		isLoading = true;
		curStep = 0;
	}
	else {
		isLoading = false;
		curStep = 0;
	}
	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->schedule(std::bind(&ResourceLoader::updateLoading, this, placeholders::_1), this, 0, CC_REPEAT_FOREVER, 0, false, LOADER_SCHEDULER);

}

void ResourceLoader::updateLoading(float dt) {
	auto totalSteps = loadedObjVec.size();
	if (curStep < totalSteps) {
		auto &loadedObj = loadedObjVec[curStep];
		loadedObj.load();
		bool suc = loadedObj.succeed; // Luu ket qua cua loading

		if (!suc) 
			loadedObjVec.erase(loadedObjVec.begin() + curStep); // Xoa khoi vector
		else 
			++curStep;
		
		// Trigger Event khi load xong 1 file
		EVENT_CUSTOM::EC_LoadStepFinishedData ret = { loadedObj.fname, loadedObj.succeed };
		auto eventDispatcher = Director::getInstance()->getEventDispatcher();
		eventDispatcher->dispatchCustomEvent(EVENT_CUSTOM::RES_LOADING_STEP_FINISHED, &ret);
		return;
	}

	if (curStep >= totalSteps) { // Loading ket thuc
		curStep = 0;
		isLoading = false;
		loadedObjVec.clear();

		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->unschedule(LOADER_SCHEDULER, this); // Dung cap nhat loading

		// Trigger Event(hoan tat loading)
		EVENT_CUSTOM::EC_LoadingFinishedData ret;
		auto eventDispatcher = Director::getInstance()->getEventDispatcher();
		eventDispatcher->dispatchCustomEvent(EVENT_CUSTOM::RES_LOADING_FINISHED, &ret);
	}
	return;
}
//}
RESOURCE_LOADER_NS_END


