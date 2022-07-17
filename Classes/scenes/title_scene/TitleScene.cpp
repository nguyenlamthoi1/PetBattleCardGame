#include "TitleScene.h"
#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "define/CommonDefine.h"
#include "ui/UIHelper.h"

#include <new>

USING_NS_CC;

using namespace std;

TitleScene* TitleScene::create() {
	auto scn = new (std::nothrow) TitleScene();
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

TitleScene::TitleScene(){}

TitleScene::~TitleScene(){}

bool TitleScene::init() {
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	auto ui = static_cast<ui::Layout*>(resPool->tryGetNodeCsb("ccstd_csb/title_scene/title_scene.csb"));
	ui->setContentSize(winSize);
	ui::Helper::doLayout(ui);
	this->addChild(ui);
	return true;
}

void TitleScene::onEnter() {
	Scene::onEnter();
}

void TitleScene::onExit() {
	Scene::onExit();
}