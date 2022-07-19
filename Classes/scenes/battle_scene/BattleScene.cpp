#include "TitleScene.h"
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

TitleScene::~TitleScene(){
	CCLOG("TitleScene::dtor called");
	doClear();
}

void TitleScene::doClear() {
	for (const auto& text : texts)
		text->removeFromParent(); // Delete cac text
	texts.clear();

	ui->removeFromParent();
	PoolVector::returnNode(ui);
}


bool TitleScene::init() {
	if (!Scene::init())
		return false;

	// Khoi tao data cho cac option
	vector<Option> opv; 
	auto lang = GameManager::getInstance()->getLang();
	// Page 0
	opv.emplace_back(make_pair(lang->getString("TXT_TITLE_SCENE_START_GAME"), [this]() {doPlayGame(); }));
	opv.emplace_back(make_pair(lang->getString("TXT_TITLE_SCENE_NEW_GAME"), [this]() {showOptionPage(PAGE_1);}));
	opv.emplace_back(make_pair(lang->getString("TXT_TITLE_SCENE_LANGUAGE"), [this]() {showOptionPage(PAGE_2);}));
	opv.emplace_back(make_pair(lang->getString("TXT_TITLE_SCENE_SETTING"), [this]() {showOptionPage(PAGE_3); }));
	opv.emplace_back(make_pair(lang->getString("TXT_TITLE_SCENE_QUIT_GAME"), [this]() { doQuitGame(); }));
	pages.insert({PAGE_0, opv});
	opv.clear();
	// Page 1
	opv.emplace_back(make_pair("Opt 4", [this]() {}));
	opv.emplace_back(make_pair("Opt 5", [this]() {}));
	opv.emplace_back(make_pair("Back", [this]() { showOptionPage(PAGE_0); }));
	pages.insert({ PAGE_1, opv });
	opv.clear();
	// Page 2
	opv.emplace_back(make_pair("English", [this]() {doLangSet(); }));
	opv.emplace_back(make_pair("Viet Nam", [this]() {doLangSet(); }));
	opv.emplace_back(make_pair("Back", [this]() { showOptionPage(PAGE_0);}));
	pages.insert({ PAGE_2, opv });
	opv.clear();
	// Page 3
	opv.emplace_back(make_pair("Display", [this]() {doPlayGame(); }));
	opv.emplace_back(make_pair("Sound", [this]() {doPlayGame(); }));
	opv.emplace_back(make_pair("Back", [this]() { showOptionPage(PAGE_0); }));
	pages.insert({ PAGE_3, opv });
	opv.clear();
	//--


	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	ui = static_cast<ui::Layout*>(resPool->tryGetNodeCsb("ccstd_csb/title_scene/title_scene.csb"));
	if (!ui)
		return false;
	
	ui->setContentSize(winSize);
	ui::Helper::doLayout(ui);
	this->addChild(ui);
	return true;
}

void TitleScene::onEnter() {
	Scene::onEnter();
	showOptionPage(PAGE_0);
}

void TitleScene::onExit() {
	Scene::onExit();
}

void TitleScene::showOptionPage(unsigned char pageId) {
	if (pages.find(pageId) != pages.cend()) {
		const auto &v = pages[pageId];
		showOptions(v);
	}
}

void TitleScene::showOptions(const vector<Option> &opVec) {
	constexpr unsigned char MAX_OP_NUM = 5;
	
	if (opVec.size() > texts.size()) // Them vao nhung Text object con thieu
	{
		auto bgPanel = ui->getChildByName("Background_Panel");
		auto createdNum = opVec.size() - texts.size();
		for (size_t i = 0; i < createdNum; ++i) {
			auto newText = ui::Text::create("", "fonts/pokemon_pixel_font.ttf", 48);
			bgPanel->addChild(newText);
			texts.push_back(newText);
		}
	}

	CCASSERT(texts.size() >= opVec.size(), "TitleScene::showOptions: texts.size() must equal or greater than opVect.size()");

	auto dir = Director::getInstance();
	auto visibleSize = dir->getVisibleSize();
	float defaultX = visibleSize.width / 2;
	float y = 330.0f;
	constexpr float dY = 60.0f;
	for (decltype(texts.size()) i = 0; i != opVec.size(); ++i) {
		auto textNode = *(texts.begin() + i);
		auto option = *(opVec.begin() + i);
		auto &s = option.first;
		auto &clickedFunc = option.second;
		if (textNode) {
			textNode->setString(s);
			textNode->setPosition(Vec2(defaultX, y));
			y -= dY;
			textNode->setVisible(true);
			textNode->setColor(Color3B::BLACK);

			textNode->setSwallowTouches(true);
			textNode->setTouchEnabled(true);
			setWidgetTouchEnded(textNode, 
				[clickedFunc](WIDGET_TOUCH_HANDLER_PARAMS) { 
				if(clickedFunc) 
					clickedFunc(); 
				});
		}
	}

	size_type i = min(opVec.size(), static_cast<size_t>(MAX_OP_NUM));
	while (i < texts.size())  // Tat visible nhung text node con lai
		texts[i++]->setVisible(false);
}



void TitleScene::doPlayGame() {
	CCLOG("TitleScene: start game");
}
void TitleScene::doContinue() {
	CCLOG("TitleScene: continue game");
}
void TitleScene::doQuitGame() {
	CCLOG("TitleScene: quit game");
}
void TitleScene::doLangSet() {
	CCLOG("TitleScene: set language");
}

