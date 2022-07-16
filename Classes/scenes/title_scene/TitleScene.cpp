#include "TitleScene.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "define/CommonDefine.h"

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



	return true;
}

void TitleScene::onEnter() {
	Scene::onEnter();
}

void TitleScene::onExit() {
	Scene::onExit();
}