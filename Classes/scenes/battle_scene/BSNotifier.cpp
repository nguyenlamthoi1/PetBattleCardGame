#include "BSNotifier.h"
#include "BattleScene.h"

#include "GameManager.h"
#include "common/ResourcePool.h"

#include <new>

USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSNotifier* BSNotifier::create(BattleScene *scn) {
	auto notifier = new (nothrow) BSNotifier(scn);

	if (notifier && notifier->init())
		return notifier;
	
	delete notifier;
	notifier = nullptr;
	return notifier;
}

BSNotifier::BSNotifier(BattleScene *scn) : btlScn(scn) {
	CCLOG("BSNotifier::Ctor %p", this);
}

BSNotifier::~BSNotifier() {
	CCLOG("BSNotifier::Dtor %p", this);
}

bool BSNotifier::init() {
	if (!btlScn)
		return false;

	//root = GM_POOL->tryGetNodeCsb("ccstd_csb/battle_scene/Msg_Layer.csb");
	root = btlScn->getBattleSceneUI()->getChildByName("Msg_Layer");

	if (!root)
		return false;
	root->setPosition(Vec2::ZERO);

	msgBoard = dynamic_cast<ui::Layout*>(root->getChildByName("Msg_Board"));
	msgBoard->setVisible(false);

	doneBtn = dynamic_cast<ui::Button*>(root->getChildByName("Done_Button"));
	doneBtn->setVisible(false);

	return true;
}

void BSNotifier::showMsg(const string &msg) {
	setMsg(msg);
	msgBoard->setVisible(true);
	msgBoard->setOpacity(0);
	msgBoard->runAction(
		FadeIn::create(0.3f)
	);
}

void BSNotifier::hideMsg(float afterT) {
	if (msgBoard->isVisible()) {
		msgBoard->setOpacity(255);
		msgBoard->runAction(
			Sequence::create(
				DelayTime::create(afterT),
				FadeOut::create(0.3f),
				CallFunc::create([this]() {msgBoard->setVisible(false); }),
				nullptr)
		);
	}
}

void BSNotifier::showMsgAndHideAfter(const std::string &msg, float t) {
	showMsg(msg);
	hideMsg(t);
}
void BSNotifier::showMsgWithDone(const std::string &msg, DoneFunc f, bool hideDoneOnClick) {
	showMsg(msg);
	doneBtn->setVisible(true);
	doneBtn->addClickEventListener([this, f, hideDoneOnClick](Ref *btn) {
		if (f)
			f();
		if (hideDoneOnClick)
			doneBtn->setVisible(false);
		});
}

void BSNotifier::setMsg(const string &msg) {
	auto text = dynamic_cast<ui::Text*>(msgBoard->getChildByName("Msg_Text"));
	text->setString(msg);
	auto textSize = text->getContentSize();
	auto boardSize = msgBoard->getContentSize();

	constexpr float D_WIDTH = 20;
	constexpr float D_HEIGHT = 10;

	float newW = textSize.width + D_WIDTH * 2;
	float newH = textSize.height + D_HEIGHT * 2;
	msgBoard->setContentSize(Size(newW, newH));

	auto layPara = ui::RelativeLayoutParameter::create();
	//layPara->setMargin(ui::Margin(D_WIDTH, D_HEIGHT, D_WIDTH, D_HEIGHT));
	layPara->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
	text->setLayoutParameter(layPara);

	msgBoard->setLayoutType(ui::Layout::Type::RELATIVE);
	msgBoard->requestDoLayout();
}

BATTLE_SCENE_NS_END