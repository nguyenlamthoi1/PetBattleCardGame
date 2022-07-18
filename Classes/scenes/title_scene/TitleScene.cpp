#include "TitleScene.h"
#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
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
	showFirstOptions();
}

void TitleScene::onExit() {
	Scene::onExit();
}

void TitleScene::showFirstOptions() {
	showOptions(
		{
			make_pair("Opt 1", [this](){
					showOptions(
						{
							make_pair("Opt 1", [this]() { doPlayGame(); }),
							make_pair("Opt 2", [this]() { doContinue(); }),
							make_pair("Opt 3", [this]() { doQuitGame(); }),
							make_pair(GM_LANG->getString("TXT_TITLE_SCENE_BACK"), [this]() {showPrevOptions(); })
						});
			}),
			make_pair("Quit game", [this]() { doQuitGame(); })
		});
}


void TitleScene::showOptions(vector<Option> opVect) {
	auto lang = GameManager::getInstance()->getLang();
	constexpr unsigned char MAX_OP_NUM = 4;

	//vector<Option> opVect(opts.begin(), opts.end());
	if(opVect.size() > MAX_OP_NUM) // Xoa option bi du
		opVect.erase(opVect.begin() + MAX_OP_NUM, opVect.end());
	
	preOpVec = curOpVec; // Luu lai opVec hien tai

	if (opVect.size() > texts.size()) // Them vao nhung Text object con thieu
	{
		auto bgPanel = ui->getChildByName("Background_Panel");
		auto n = opVect.size() - texts.size();
		for (size_t i = 0; i < n; i++) {
			auto newText = ui::Text::create("", "fonts/pokemon_pixel_font.ttf", 48);
			bgPanel->addChild(newText);
			texts.push_back(newText);
		}
	}

	CCASSERT(texts.size() == opVect.size(), "TitleScene::showOptions: texts and options don't have same size");

	auto dir = Director::getInstance();
	auto visibleSize = dir->getVisibleSize();
	float defaultX = visibleSize.width / 2;
	float y = 330.0f;
	constexpr float dY = 60.0f;
	for (decltype(texts.size()) i = 0; i != opVect.size(); ++i) {
		auto textNode = *(texts.begin() + i);
		auto option = *(opVect.begin() + i);
		auto &s = option.first;
		auto &clickedFunc = option.second;
		if (textNode) {
			textNode->setString(s);
			textNode->setPosition(Vec2(defaultX, y));
			y -= dY;
			textNode->setVisible(true);
			textNode->setColor(Color3B::BLACK);

			// TODO: Attach clicked callback
		}
	}

	size_type i = opVect.size();
	while (i < texts.size())  // Tat visible nhung text node con lai
		texts[i]->setVisible(false);
	
}

void TitleScene::showPrevOptions() {
	showOptions(preOpVec);
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