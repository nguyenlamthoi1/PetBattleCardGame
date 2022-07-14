#include "LoadingScene.h"

USING_NS_CC;

LoadingScene* LoadingScene::create()
{
	LoadingScene *scn = new LoadingScene();
	if (scn->init())
	{
		scn->autorelease();
		return scn;
	}
	else
	{
		delete scn;
		scn = nullptr;
		return nullptr;
	}
}

bool LoadingScene::init() {
	if (!Scene::init()) 
		return false;
	return true;
}

LoadingScene::LoadingScene() {
}

LoadingScene::~LoadingScene() {

}

void LoadingScene::onEnter() {
	Scene::onEnter();

	showLogoUI();
}

void LoadingScene::onExit() {
	Scene::onExit();
}

void LoadingScene::showLogoUI() {
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	if (!layout) {
		layout = ui::Layout::create();
		layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
		layout->setBackGroundColorOpacity(255);
		layout->setBackGroundColor(Color3B(250, 214, 184));
		layout->setContentSize(visibleSize); // Xet bang kich thuoc man hinh
	
		auto logoSprite = Sprite::create("single_images/logo_with_shadow.png");
		Size logoSize = logoSprite->getContentSize();
		logoSprite->setName("logoSprite");
		logoSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height /2));
		logoSprite->setScale(1.5f);
		layout->addChild(logoSprite);

		auto circleSprite = Sprite::create("single_images/hud_loading_circle.png");
		Size circleSize = circleSprite->getContentSize();
		constexpr float circleMarginRight = 200.0f;
		constexpr float circleMarginBottom = 200.0f;
		circleSprite->setName("CircleSprite");
		circleSprite->setPosition(Vec2(visibleSize.width - circleSize.width / 2 - circleMarginRight, visibleSize.height + circleSize.height / 2 + circleMarginBottom));
		layout->addChild(circleSprite);

		bottomLabel = ui::Text::create("", "fonts\Pokemon_Pixel_Font", 36);
		bottomLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - logoSize.height / 2 - 300.0f));
		bottomLabel->setTextColor(Color4B::WHITE);
		layout->addChild(bottomLabel);

		this->addChild(layout);
	}
	
	
}


