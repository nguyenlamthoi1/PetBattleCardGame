#include "MoveHolder.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/BSPrizePile.h"
#include "scenes/battle_scene_3/CardHolder.h"
#include "scenes/battle_scene_3/prefabs/BSPrefabs.h"

#include "data/MoveData.h"

#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
using namespace std;
BS_PREFABS_USE_NS;

BATTLE_SCENE_NS_BEG

MoveHolder* MoveHolder::create(const shared_ptr<const MoveData> &data) {
	auto ret = new (nothrow) MoveHolder(data);
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	ret = nullptr;
	return ret;
}

MoveHolder::MoveHolder(const shared_ptr<const MoveData> &dta) : data(dta) {}

MoveHolder::~MoveHolder(){
	cleanUI();
}

bool MoveHolder::init() {
	root = GM_POOL->tryGetNodeCsb("ccstd_csb/battle_scene/prefabs/MoveInfo.csb");
	if (!root)
		return false;

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	moveNameLb = dynamic_cast<ui::Text*>(container->getChildByName("Name"));
	dmgLb = dynamic_cast<ui::Text*>(container->getChildByName("DmgText"));
	useBtn = dynamic_cast<ui::Button*>(container->getChildByName("UseBtn"));
	energyPanel = dynamic_cast<ui::Layout*>(container->getChildByName("EnergyPanel"));
	desBoard = dynamic_cast<ui::Layout*>(container->getChildByName("Move_Board"));

	auto moveName = data->name;
	moveNameLb->setString(moveName);
	auto dmgText = data->damageText;
	dmgLb->setString(dmgText);
	
	setMoveDes(data->content);

	this->setContentSize(Size(500, 130));
	this->addChild(root);

	auto &costMap = data->costMap;
	for (const auto &itr : costMap) {
		auto eType = itr.first;
		auto eNum = itr.second;
		auto eItem = EnergyItem::create(eType, eNum);
		energyPanel->addChild(eItem);
	}
	root->setPosition(Vec2(10, 5));
	updateEnergyPanel();

	useBtn->addClickEventListener([this](Ref* sender){
		if (onUseMove)
			onUseMove(this);
		});

	return true;
}

void MoveHolder::cleanUI() {
	energyPanel->removeAllChildren();
	if (root) {
		root->removeFromParent(); // Release
		PoolVector::returnNode(root);
		root = nullptr;
	}
}

void MoveHolder::setMoveDes(const std::string &s) {
	auto desText = dynamic_cast<ui::Text*>(desBoard->getChildByName("Text"));
	desText->setString(s);
	//TODO
}

void MoveHolder::updateEnergyPanel() {
	auto children = energyPanel->getChildren();
	unsigned int i = 0;
	constexpr float spaceX = 10.0f;
	for (const auto &child : children) {
		auto eChild = dynamic_cast<EnergyItem*>(child);
		auto eSize = eChild->getEnergySize();
		child->setPositionX((eSize.width / 2 + spaceX) * i);
		child->setPositionY(0);
		++i;
	}
}

const std::map<std::string, unsigned int>& MoveHolder::getRequiredEnergies() const {
	return data->costMap;
}

void MoveHolder::setEnabledUseBtn(bool enabled) {
	useBtn->setVisible(enabled);
}



BATTLE_SCENE_NS_END