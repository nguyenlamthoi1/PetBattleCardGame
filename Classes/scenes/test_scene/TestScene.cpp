#include "TestScene.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "components/WidgetTouchComponent.h"
#include "define/CommonDefine.h"
#include "scenes/battle_scene_3/BattleScene.h"
#include "ui/UIHelper.h"

#include <new>
#include <algorithm>

USING_NS_CC;
WIDGET_TOUCH_USE_NS;
//BATTLE_SCENE_USE_NS;
using namespace std;

TestScene* TestScene::create() {
	auto scn = new (std::nothrow) TestScene();
	if (scn && scn->init()) {
		scn->autorelease();
		return scn;
	}
	else {
		delete scn;
		scn = nullptr;
	}
	
	return scn;
}

TestScene::TestScene(){}

TestScene::~TestScene(){
	if (root) {
		root->removeFromParent();
		ResourcePool::returnNode(root);
	}
}

bool TestScene::init() {
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	root = resPool->tryGetNodeCsb("ccstd_csb/battle_scene/battle_scene.csb");
	if (!root)
		return false;
	
	root->setContentSize(winSize);
	ui::Helper::doLayout(root);
	this->addChild(root);
	return true;
}

void TestScene::onEnter() {
	Scene::onEnter();
}

void TestScene::onExit() {
	Scene::onExit();
}
