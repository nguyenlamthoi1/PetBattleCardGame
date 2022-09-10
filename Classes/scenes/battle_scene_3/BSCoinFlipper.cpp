#include "BSCoinFlipper.h"
#include "BattleScene.h"

#include "GameManager.h"
#include "common/ResourcePool.h"
#include "data/PlayerData.h"
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

BSCoinFlipper::BSCoinFlipper(BattleScene *scn) : btlScn(scn) {}

BSCoinFlipper::~BSCoinFlipper() {
	//Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

bool BSCoinFlipper::init() {
	if (!btlScn)
		return false;

	root = btlScn->getScnRoot()->getChildByName("Flip_Coin_Layer");

	if (!root)
		return false;
	root->setPosition(Vec2::ZERO);

	flip1Panel = root->getChildByName("1_Flip_Panel"); flip1Panel->setVisible(false);
	flipMulPanel = root->getChildByName("Multiple_Flip_Panel"); flipMulPanel->setVisible(false);
	coinSprite = dynamic_cast<Sprite*>(root->getChildByName("Coin_Marker")->getChildByName("Coin_Sprite")); coinSprite->setVisible(true);

	const auto &pids = btlScn->getPids();
	for (const auto pid : pids) {
		auto pData = btlScn->getPlayerData(pid);
		flipperDatas.insert({ pid, make_shared<FlipperData>(pid, pData->getUsedCoin()) });
	}

	return true;
}

const string BSCoinFlipper::FLIP_BEGIN_ANIM = "FLIP_BEGIN_ANIM";
const string BSCoinFlipper::FLIP_END_ANIM = "FLIP_END_ANIM";

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

	auto &flipData = flipperDatas[whoFlip];
	const auto &usedCoin = flipData->usedCoin;
	auto &flipVec = flipData->flipVec;
	
	string color = "";
	if (usedCoin == RAICHU) {
		color = "yellow";
		flipData->SFNHeads = "flip_coin_animations/yellow/flip_coin_yellow_raichu.png";
		flipData->SFNTails = "flip_coin_animations/yellow/flip_coin_yellow_3.png";
	}
	else { // Default Pikachu
		color = "yellow";
		flipData->SFNHeads = "flip_coin_animations/yellow/flip_coin_yellow_pikachu.png";
		flipData->SFNTails = "flip_coin_animations/yellow/flip_coin_yellow_3.png";
	}

	// Tao animation
	flipVec.clear();
	char sfn[100];
	flipVec.emplace_back(flipData->SFNHeads);
	for (unsigned int i = 0; i < 7; i++) {
		sprintf(sfn, format, color.c_str(), color.c_str(), i);
		flipVec.emplace_back(sfn);
	}
}


////////////////////
////Flip 1 Coins///
///////////////////

void BSCoinFlipper::startFlip1Coin(const PlayerIdType &whoFlip, SideType resSide) {
	root->setVisible(true);
	// Reset ket qua flip truoc do
	auto &flipData = flipperDatas.at(whoFlip);
	flipData->setFlipResult({ resSide });
	//Bat dau
	currentFlipper = whoFlip;
	curFlipType = FlipType::Flip_1;

	auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
	resBoard->setOpacity(0);
	resBoard->setVisible(true);

	flip1Panel->setScale(0.75f);
	flip1Panel->setVisible(true);
	flip1Panel->setOpacity(0);

	flip1Panel->runAction(Sequence::create(
		Spawn::create(
			FadeIn::create(0.3f),
			ScaleTo::create(0.3f, 1.0f),
			nullptr),
		DelayTime::create(0.5f),
		CallFunc::create([this, whoFlip, resSide]() { doFlip1Coin_Flip1Type(whoFlip, resSide); }),
		nullptr));

	coinSprite->setVisible(true);
	coinSprite->setOpacity(0);
	coinSprite->runAction(FadeIn::create(0.3f));

}

void BSCoinFlipper::doFlip1Coin_Flip1Type(const PlayerIdType &whoFlip, SideType flippedSide) {
	auto &flipData = flipperDatas[whoFlip];

	bool gotHead = flippedSide == HEADS;

	if (flipData->flipVec.empty())
		createFlipAnimation(whoFlip);

	auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
	if (!comp)
		comp = SpriteAnimatorComponent::setComponent(coinSprite);
	// Add Animations
	comp->addAnimation(FLIP_BEGIN_ANIM, flipData->flipVec, 1.0f / 8); // FLIP_BEGIN_ANIM
	flipData->flipVec.emplace_back(gotHead ? flipData->SFNHeads : flipData->SFNTails);
	comp->addAnimation(FLIP_END_ANIM, flipData->flipVec, 1.0f / 9); // FLIP_END_ANIM
	flipData->flipVec.pop_back();
	//--
	comp->playAnimation(FLIP_BEGIN_ANIM, true); // * play FLIP_BEGIN_ANIM animation

	auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
	resBoard->setVisible(true);
	resBoard->setOpacity(0);

	coinSprite->setPosition(Vec2(0, 0));
	coinSprite->runAction(Sequence::create(
		MoveBy::create(0.5f, Vec2(0, 45)), // * Must be 0.5f
		MoveBy::create(0.5f, Vec2(0, -45)), // * Must be 0.5f
		CallFunc::create([this, gotHead, whoFlip]() {
			auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
			comp->playAnimation(FLIP_END_ANIM, false, [this, gotHead, whoFlip](string anim) {
				// Ket thuc 1 lan flip
				auto resBoard = flip1Panel->getChildByName("Result_Flip_Board");
				resBoard->runAction(FadeIn::create(0.5f));
				auto resText = dynamic_cast<ui::Text*>(resBoard->getChildByName("Result_Flip_Text"));
				resText->setString(gotHead ? "Heads" : "Tails");

				onFlipActionDone();
				});
			}),
		nullptr));
}


////////////////////////
////////////////////////
////////////////////////


//////////////////////////
////Flip Multiple Coins///
//////////////////////////

void BSCoinFlipper::startFlipMulCoins(PlayerIdType whoFlip, unsigned int n) {
	//CCASSERT(whoFlip == PLAYER || whoFlip == OPPONENT, "BSCoinFlipper::startFlip1Coin: wrong id");

	//if (n < 1)
	//	return;

	//root->setVisible(true);

	//// Reset ket qua flip truoc do
	//auto &flipInfo = flipInfos[whoFlip];
	//flipInfo.clear();
	//// Tao ket qua moi
	//for (unsigned int i = 0; i < n; ++i) {
	//	SideType resSide = RandomHelper::random_int((int)TAILS, (int)HEADS); // 0 hoac 1
	//	flipInfo.resVec.push_back(resSide);
	//}
	////Bat dau
	//currentFlipper = whoFlip;
	//curFlipType = FlipType::Flip_Mul;
	//auto flipCount = flipInfo.resVec.size();

	//auto titleText = dynamic_cast<ui::Text*>(flipMulPanel->getChildByName("Flip_Text"));
	//titleText->setString(StringUtils::format("Flip %d ", flipCount) + (flipCount > 1 ? "Coins" : "Coin"));
	//auto headsText = dynamic_cast<ui::Text*>(flipMulPanel->getChildByName("Num_Heads_Board")->getChildByName("Num_Heads_Text"));
	//headsText->setString("Heads : 0");
	//auto tailsText = dynamic_cast<ui::Text*>(flipMulPanel->getChildByName("Num_Tails_Board")->getChildByName("Num_Tails_Text"));
	//tailsText->setString("Tails : 0");

	//flipMulPanel->setScale(0.75f);
	//flipMulPanel->setVisible(true);
	//flipMulPanel->setOpacity(0);
	//flipMulPanel->runAction(Sequence::create(
	//	Spawn::create(
	//		FadeIn::create(0.3f),
	//		ScaleTo::create(0.3f, 1.0f),
	//		nullptr),
	//	DelayTime::create(0.5f),
	//	CallFunc::create([this, whoFlip]() {
	//		doFlip1Coin_FlipMulType(whoFlip);
	//		}),
	//	nullptr
	//));
}

void BSCoinFlipper::doFlip1Coin_FlipMulType(PlayerIdType whoFlip) {
	//auto &flipInfo = flipInfos[whoFlip];

	//if (flipInfo.resVec.empty()) {
	//	CCLOG("BSCoinFlipper::doFlip1Coin: wrong resVec");
	//	return; //->error
	//}

	//SideType flippedSide = *(flipInfo.resVec.cbegin() + flipInfo.doneCount);
	//bool gotHead = flippedSide != TAILS;

	//if (flipInfo.flipVec.empty())
	//	createFlipAnimation(whoFlip);

	//auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
	//if (!comp)
	//	comp = SpriteAnimatorComponent::setComponent(coinSprite);
	//// Add Animations
	//comp->addAnimation(FLIP_BEGIN_ANIM, flipInfo.flipVec, 1.0f / 8); // FLIP_BEGIN_ANIM

	//flipInfo.flipVec.emplace_back(gotHead ? flipInfo.SFNHeads : flipInfo.SFNTails);
	//comp->addAnimation(FLIP_END_ANIM, flipInfo.flipVec, 1.0f / 9); // FLIP_END_ANIM
	//flipInfo.flipVec.pop_back();
	////--
	//comp->playAnimation(FLIP_BEGIN_ANIM, true); // * play FLIP_BEGIN_ANIM animation

	//coinSprite->setPosition(Vec2(0, 0));
	//coinSprite->runAction(Sequence::create(
	//	MoveBy::create(0.5f, Vec2(0, 45)), // * Must be 0.5f
	//	MoveBy::create(0.5f, Vec2(0, -45)), // * Must be o.5f
	//	CallFunc::create([this, flippedSide, whoFlip]() {
	//		auto comp = SpriteAnimatorComponent::getComponent(coinSprite);
	//		comp->playAnimation(FLIP_END_ANIM, false, [this, flippedSide, whoFlip](string anim) {

	//			auto &flipInfo = flipInfos[whoFlip];
	//			++flipInfo.doneCount;
	//			flipInfo.result[flippedSide] += 1;

	//			auto headsText = dynamic_cast<ui::Text*>(flipMulPanel->getChildByName("Num_Heads_Board")->getChildByName("Num_Heads_Text"));
	//			headsText->setString(string("Heads : ") + to_string(flipInfo.result[HEADS]));
	//			auto tailsText = dynamic_cast<ui::Text*>(flipMulPanel->getChildByName("Num_Tails_Board")->getChildByName("Num_Tails_Text"));
	//			tailsText->setString(string("Tails : ") + to_string(flipInfo.result[TAILS]));

	//			if (flipInfo.doneCount == flipInfo.resVec.size())  // Ket thuc flip
	//				onFlipActionDone();
	//			else
	//				callDelayFunc([this, whoFlip](float t) {doFlip1Coin_FlipMulType(whoFlip); }, 0.5f);
	//			});
	//		}),
	//	nullptr));
}

//////////////////////////
//////////////////////////
//////////////////////////

const string BSCoinFlipper::EV_FLIP_DONE = "EV_FLIP_DONE";

void BSCoinFlipper::onFlipActionDone() {
	// * Khong can reset, vi de get data cua FlipAction vua moi duoc thuc hien
	//currentFlipper = ""; 
	//curFlipType = FlipType::Flip_1;
	//--
	//CCLOG("Flip 1 done");
	//notifyEvent(EventType::Flip_Action_Ended);

	auto evData = EV_FlipDoneData({ curFlipType });
	dispatchEvent(EV_FLIP_DONE, &evData);
}

void BSCoinFlipper::notifyEvent(EventType ev) {
	switch (ev) {
	case EventType::Flip_Action_Ended:
		for (const auto &f : onFlipEndedOnce)
			f();
		onFlipEndedOnce.clear(); // Xoa callback sau khi call
		break;
	}
}

void BSCoinFlipper::registFlipEndCallbackOnce(const OnFlipEnded &f) {
	onFlipEndedOnce.push_back(f);
}

const string BSCoinFlipper::DELAY_SCHEDULER = "DELAY_SCHEDULER";

void BSCoinFlipper::callDelayFunc(const DelayFunc &f, float t) {
	Director::getInstance()->getScheduler()->schedule(f, this, 0, 0, t, 0, DELAY_SCHEDULER);
}

void BSCoinFlipper::hideFlip1(float afterDt) const {
	coinSprite->runAction(
		Sequence::create(
			DelayTime::create(afterDt),
			FadeOut::create(0.3f),
			CallFunc::create([this]() {
				coinSprite->setVisible(false);
				coinSprite->setOpacity(255);
				}),
			nullptr
					));
	flip1Panel->runAction(
		Sequence::create(
			DelayTime::create(afterDt),
			FadeOut::create(0.3f),
			CallFunc::create([this]() {
				flip1Panel->setVisible(false);
				flip1Panel->setOpacity(255);
				}),
			nullptr
		));
}

//-----------------//
//FlipperData Class//
//-----------------//

void BSCoinFlipper::FlipperData::setFlipResult(initializer_list<SideType> vec) {
	resVec.assign(vec.begin(), vec.end());

	resMap.clear();
	for (const auto &side : vec) 
		resMap[side] = resMap[side] + 1;
}

void BSCoinFlipper::FlipperData::setFlipResult(const vector<SideType> &vec) {
	resVec.assign(vec.begin(), vec.end());

	resMap.clear();
	for (const auto &side : vec)
		resMap[side] = resMap[side] + 1;
}

void BSCoinFlipper::FlipperData::clearResult() {
	resVec.clear();
	resMap.clear();
}


BATTLE_SCENE_NS_END