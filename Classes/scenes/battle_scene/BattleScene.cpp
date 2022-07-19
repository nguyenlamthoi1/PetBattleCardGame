#include "BattleScene.h"
#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "components/WidgetTouchComponent.h"
#include "define/CommonDefine.h"

#include "ui/UIHelper.h"

#include <new>
#include <algorithm>

USING_NS_CC;
WIDGET_TOUCH_USE_NS;
using namespace std;

BattleScene* BattleScene::create() {
	auto scn = new (std::nothrow) BattleScene();
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

BattleScene::BattleScene(){}

BattleScene::~BattleScene(){
	CCLOG("BattleScene::dtor called");
}

bool BattleScene::init() {
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	ui = static_cast<ui::Layout*>(resPool->tryGetNodeCsb("ccstd_csb/battle_scene/battle_scene.csb"));
	if (!ui)
		return false;
	
	ui->setContentSize(winSize);
	ui::Helper::doLayout(ui);
	this->addChild(ui);
	return true;
}

void BattleScene::onEnter() {
	Scene::onEnter();
}

void BattleScene::onExit() {
	Scene::onExit();
}

