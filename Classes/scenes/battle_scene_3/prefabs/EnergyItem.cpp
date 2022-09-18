#include "BSPrefabs.h"

#include "GameManager.h"
#include "common/ResourcePool.h"

USING_NS_CC;
using namespace std;

BS_PREFABS_NS_BEG

std::unordered_map<EnergyItem::EnergyId, std::string> EnergyItem::energySFNMap = {
	{CardDefine::Energy::None, "energies/x1/energy_normal.png"},
	{CardDefine::Energy::Fire, "energies/x1/energy_fire.png"},
	{CardDefine::Energy::Water, "energies/x1/energy_water.png"},
	{CardDefine::Energy::Grass, "energies/x1/energy_grass.png"},
	{CardDefine::Energy::Electric, "energies/x1/energy_electric.png"},
	{CardDefine::Energy::Psychic, "energies/x1/energy_psychic.png"},
	{CardDefine::Energy::Fighting, "energies/x1/energy_fighting.png"},
	{CardDefine::Energy::Dark, "energies/x1/energy_dark.png"},
	{CardDefine::Energy::Dragon, "energies/x1/energy_dragon.png"},
	{CardDefine::Energy::Normal, "energies/x1/energy_normal.png"},
	{CardDefine::Energy::Steel, "energies/x1/energy_steel.png"}
};

EnergyItem* EnergyItem::create(EnergyId eid, unsigned int c) {
	auto ret = new (nothrow) EnergyItem(eid, c);
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	ret = nullptr;
	return ret;
}

EnergyItem::EnergyItem(EnergyId eid, unsigned int c) : id(eid), count(c) {}

EnergyItem::~EnergyItem(){
	if (root) {
		if (root->getReferenceCount() > 1) {
			root->removeFromParent(); // Release
			PoolVector::returnNode(root);
		}
		root = nullptr;
	}
}

bool EnergyItem::init() {
	if (!Node::init())
		return false;

	root = GM_POOL->tryGetNodeCsb("ccstd_csb/battle_scene/prefabs/energy_item.csb");
	if (!root)
		return false;


	energySprite = dynamic_cast<Sprite*>(root->getChildByName("EnergySprite"));
	if (!energySprite)
		return false;

	energyText = dynamic_cast<ui::Text*>(energySprite->getChildByName("EnergyText"));
	if (!energyText)
		return false;

	energySprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(energySFNMap[id]));
	energyText->setString(count > 1 ? to_string(count) : "");


	this->addChild(root);
	root->setPosition(Vec2(0, 0));
	//auto eSize = energySprite->getContentSize();
	//this->setAnchorPoint(Vec2(0.5f, 0.5f));
	//this->setContentSize(eSize);

	return true;
}

void EnergyItem::stack(unsigned int inc, bool doAnim) {
	count += inc;
	energyText->setString(count > 1 ? to_string(count) : "");

	if (doAnim) {
		this->stopAllActions();
		this->setScale(1.0f);
		auto toScale1 = this->getScale() * 1.4f;
		auto toScale2 = this->getScale() * 0.8f;
		auto toScale3 = this->getScale() * 1.0f;

		this->runAction(Sequence::create(
			ScaleTo::create(0.35f, toScale1),
			ScaleTo::create(0.3f, toScale2),
			ScaleTo::create(0.3f, toScale3),
			nullptr));
	}
}

Size EnergyItem::getEnergySize() const {
	return energySprite->getContentSize();
}

BS_PREFABS_NS_END