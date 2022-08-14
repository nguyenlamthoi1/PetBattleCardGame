#include "BSCoinFlipper.h"
#include "BattleScene.h"

#include "GameManager.h"
#include "common/ResourcePool.h"

#include "components/SpriteAnimatorComponent.h"

#include <new>

USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSCoinFlipper* BSCoinFlipper::create(BattleScene *scn) {
	auto flipper = new (nothrow) BSCoinFlipper(scn);

	if (flipper && flipper->init())
		return flipper;
	
	delete flipper;
	flipper = nullptr;
	return flipper;
}

BSCoinFlipper::BSCoinFlipper(BattleScene *scn) : btlScn(scn) {
	CCLOG("BSCoinFlipper::Ctor %p", this);

	
}

BSCoinFlipper::~BSCoinFlipper() {
	CCLOG("BSCoinFlipper::Dtor %p", this);
}

bool BSCoinFlipper::init() {
	if (!btlScn)
		return false;

	//root = GM_POOL->tryGetNodeCsb("ccstd_csb/battle_scene/flip_coin_layer.csb");
	root = btlScn->getBattleSceneUI()->getChildByName("Flip_Coin_Layer");

	if (!root)
		return false;
	root->setPosition(Vec2::ZERO);

	flip1Panel = root->getChildByName("1_Flip_Panel"); flip1Panel->setVisible(false);
	flipMulPanel = root->getChildByName("Multiple_Flip_Panel"); flipMulPanel->setVisible(false);
	coinSprite = dynamic_cast<Sprite*>(root->getChildByName("Coin_Marker")->getChildByName("Coin_Sprite")); coinSprite->setVisible(true);

	auto playerData = btlScn->getPlayerData(PLAYER);
	flipInfos.insert({ PLAYER, FlipInfo({PLAYER, playerData->getUsedCoin()}) });
	flipInfos.insert({ OPPONENT, FlipInfo({OPPONENT, "GR"}) });

	return true;
}

void BSCoinFlipper::startFlip1Coin(PlayerIdType whoFlip) {
	CCASSERT(whoFlip == PLAYER || whoFlip == OPPONENT, "BSCoinFlipper::startFlip1Coin: wrong id");

	// Reset ket qua flip truoc do
	auto &flipInfo = flipInfos[whoFlip];
	flipInfo.clear();
	// Tao ket qua moi
	SideType resSide = RandomHelper::random_int((int)TAILS, (int)HEADS); // 0 hoac 1
	flipInfo.resVec.push_back(resSide);
	//Bat dau
	currentFlipper = whoFlip;
	curFlipType = FlipType::Flip_1;

	flip1Panel->setScale(0.75f);
	flip1Panel->setVisible(true);
	flip1Panel->setOpacity(0);
	flip1Panel->runAction(Sequence::create(
		Spawn::create(
			FadeIn::create(0.3f),
			ScaleTo::create(0.3f, 1.0f),
			nullptr),
		DelayTime::create(0.5f),
		CallFunc::create([this]() {
			}),
		nullptr
		));
}

void BSCoinFlipper::onFlipActionDone() {
	// * Khong can reset, vi de get data cua FlipAction vua moi duoc thuc hien
	//currentFlipper = ""; 
	//curFlipType = FlipType::Flip_1;
	//--
	CCLOG("Flip 1 done");
}

void BSCoinFlipper::createFlipAnimation(PlayerIdType whoFlip) {
	// Yellow
	static const string PIKACHU = "Pikachu";
	static const string RAICHU = "Raichu";
	static const string MAGNEMITE = "Magnemite";
	static const string LUGIA = "Lugia";

	// Red
	static const string CHARMANDER = "Charmander";
	static const string GR = "GR";
	static const string DUGTRIO = "Dugtrio";
	static const string MAGMAR = "Magmar";
	static const string PONYTA = "Ponyta";
	static const string KABUTO = "Kabuto";
	static const string MACHAMP = "Machamp";

	// Green
	static const string ARBOK = "Arbok";
	static const string GOLBAT = "Golbat";
	static const string ODDISH = "Oddish";

	// Blue
	static const string HORSEA = "Horsea";
	static const string PSYDUCK = "Psyduck";
	static const string STARMINE = "Starmine";

	// White
	static const string JIGGLYPUFF = "Jigglypuff";
	static const string SNORLAX = "Snorlax";
	static const string TOGEPI = "Togepi";

	static const char format[] = "flip_coin_animations/%s/flip_coin_%s_%d.png";
	
	auto &flipInfo = flipInfos[whoFlip];
	const auto &usedCoin = flipInfo.usedCoin;
	auto &flipVec = flipInfo.flipVec;

	string color = "";
	if (usedCoin == RAICHU) {
		color = "yellow";
		flipInfo.SFNHeads = "flip_coin_animations/yellow/flip_coin_yellow_raichu.png";
		flipInfo.SFNTails = "flip_coin_animations/yellow/flip_coin_yellow_3.png";

	}
	else { // Default Pikachu
		color = "yellow";
		flipInfo.SFNHeads = "flip_coin_animations/yellow/flip_coin_yellow_pikachu.png";
		flipInfo.SFNTails = "flip_coin_animations/yellow/flip_coin_yellow_3.png";
	}

	// Tao animation
	flipVec.clear();
	char sfn[100];
	flipVec.emplace_back(flipInfo.SFNHeads);
	for (unsigned int i = 0; i < 7; i++) {
		sprintf(sfn, format, color.c_str(), color.c_str(), i);
		flipVec.emplace_back(sfn);
	}
}

const string BSCoinFlipper::FLIP_BEGIN_ANIM = "FLIP_BEGIN_ANIM";
const string BSCoinFlipper::FLIP_END_ANIM = "FLIP_END_ANIM";

void BSCoinFlipper::doFlip1Coin(PlayerIdType whoFlip) {
	auto &flipInfo = flipInfos[whoFlip];

	if (flipInfo.resVec.size() != 1) {
		CCLOG("BSCoinFlipper::doFlip1Coin: wrong resVec");
		return; //->error
	}

	bool gotHead = *(flipInfo.resVec.cend()) != TAILS;

	if (flipInfo.flipVec.empty()) 
		createFlipAnimation(whoFlip);

	auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
	if (!comp) 
		comp = SpriteAnimatorComponent::setComponent(coinSprite);
	
	// Add Animations
	comp->addAnimation(FLIP_BEGIN_ANIM, flipInfo.flipVec, 1.0f / 8); // FLIP_BEGIN_ANIM

	flipInfo.flipVec.emplace_back(gotHead ? flipInfo.SFNHeads : flipInfo.SFNTails); 
	comp->addAnimation(FLIP_END_ANIM, flipInfo.flipVec, 1.0f / 9); // FLIP_END_ANIM
	flipInfo.flipVec.pop_back();
	//--

	auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
	resBoard->setVisible(true);
	resBoard->setOpacity(0);

	comp->playAnimation(FLIP_BEGIN_ANIM, true); // * play FLIP_BEGIN_ANIM animation
	coinSprite->setPosition(Vec2(0, 0));
	coinSprite->runAction(Sequence::create(
		MoveBy::create(0.5f, Vec2(0, 45)), // * Must be 0.5f
		MoveBy::create(0.5f, Vec2(0, -45)), // * Must be o.5f
		CallFunc::create([this, gotHead, whoFlip]() {
			auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
			comp->playAnimation(FLIP_END_ANIM, false, [this, gotHead, whoFlip](string anim) {
				// Ket thuc 1 lan flip
				auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
				resBoard->runAction(FadeIn::create(0.5f));
				auto resText = dynamic_cast<ui::Text*>(resBoard->getChildByName("Result_Flip_Text"));
				resText->setString(gotHead ? "Heads" : "Tails");

				auto &flipInfo = flipInfos[whoFlip];
				++flipInfo.doneCount;
				if (flipInfo.doneCount == flipInfo.resVec.size())  // Ket thuc flip
					onFlipActionDone();
				});
			}),
		nullptr));
}

void BSCoinFlipper::startFlipMulCoins(PlayerIdType whoFlip) {

}


BATTLE_SCENE_NS_END