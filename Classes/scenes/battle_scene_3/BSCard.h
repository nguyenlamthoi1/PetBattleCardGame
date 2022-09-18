#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ui/UIListView.h"

#include "BSDefine.h"
#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

class CardData;
class PetCardData;
class EnergyCardData;
class MoveData;

BATTLE_SCENE_NS_BEG

class BSCard : public cocos2d::ui::Layout {
public:
	enum class Type {
		None,
		Pet,
		Item,
		Energy,
		Supporter
	};

	using DragHandler = std::function<void(cocos2d::Node*, cocos2d::Node*)> ;

	static const cocos2d::Size ORIGIN_CARD_SIZE;
	static const float CARD_SCALE_DOWN;
	static cocos2d::Size CARD_SIZE;

	static BSCard* createWithData(const std::shared_ptr<const CardData> &data);
	virtual std::shared_ptr<const CardData> getData() const = 0;
	virtual void setNormalSize() = 0;
	virtual void returnToPool(){}

	// Getters and Setters
	virtual Type getType() const = 0;
	virtual bool isFlippedDown() const { return flippedDown; }
	virtual void setFlip(bool flip, bool anim = false) = 0; // Flip card ap xuong khi card dang ngua, va nguoc lai
	virtual float getFliptime() const { return flipTime; }
	//virtual void updateWithData(const std::shared_ptr<const CardData> &data);

	/*
		Hint: Them cac callback de xu ly dragCard
		- moi khi function duoc call se tu dong add 1 DragComponent, va override nhung callback truoc do
		- Luu y them danh sach destNodes de co the drag toi vi tri minh muon
	*/
	void setDragHandler(
		const std::vector<cocos2d::Node*> &destNodes,
		const DragHandler &dragBeg,
		const DragHandler &dragEnd,
		const DragHandler &dragIn = nullptr,
		const DragHandler &dragOut = nullptr);

	void setDragEnabled(bool enabled);

	virtual bool initWithData(const std::shared_ptr<const CardData> &data) = 0;

	//virtual std::string getId() const = 0;
protected:
	BSCard();
	virtual ~BSCard();
	virtual bool init() override = 0;
	virtual bool onTouchHold();

	DragHandler dragBeg = nullptr;
	DragHandler dragEnd = nullptr;
	DragHandler dragIn = nullptr;
	DragHandler dragOut = nullptr;

	bool flippedDown = false;
	float flipTime = 0.0f;
};

////////////

class PetCardMove : public cocos2d::ui::Layout
{
public:
	using CardId = std::string;

	static PetCardMove* create(const std::shared_ptr<const MoveData> &data);
	PetCardMove(const std::shared_ptr<const MoveData> &data);
	virtual ~PetCardMove();

	cocos2d::Node* getRoot() const { return root; }

	void cleanUI();

protected:

	virtual bool init();

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::Text *moveNameLb = nullptr;
	cocos2d::ui::Text *dmgLb = nullptr;
	cocos2d::ui::Layout *energyPanel = nullptr;
	cocos2d::ui::Layout *desBoard = nullptr;
	std::shared_ptr<const MoveData> data;

	void setMoveDes(const std::string &s);
	void updateEnergyPanel();
};


class PetCard : public BSCard {
public:
	virtual ~PetCard();
	static PetCard* createWithData(const std::shared_ptr<const CardData> &data);
	//static PetCard* create(const PetCardData *data);
	static PetCard* create();

	virtual bool init() override;
	virtual bool initWithData(const std::shared_ptr<const CardData> &data) override;
	virtual void returnToPool();
	virtual std::shared_ptr<const CardData> getData() const override;
	virtual void setNormalSize() override;

	std::shared_ptr<const PetCardData> data;

	cocos2d::Node *cardNode = nullptr;
	cocos2d::ui::Text *nameText = nullptr;
	cocos2d::ui::Text *hpText = nullptr;
	cocos2d::ui::ImageView *petImg = nullptr;
	cocos2d::ui::ImageView *outerImg = nullptr;
	cocos2d::ui::Layout *cardBack = nullptr;
	cocos2d::ui::Layout *cardLayout = nullptr;

	cocos2d::ui::Text *evText = nullptr;
	cocos2d::ui::ImageView *evArrow = nullptr;
	cocos2d::ui::Text *evFromText = nullptr;
	cocos2d::ui::ImageView *evFromImg = nullptr;

	cocos2d::ui::ListView *moveListView = nullptr;

	cocos2d::ui::Layout *bottomPanel = nullptr;

	cocos2d::ui::ImageView *weakPanel = nullptr;
	cocos2d::Node *weakEnergyMarker = nullptr;

	cocos2d::ui::ImageView *resistPanel = nullptr;
	cocos2d::Node *resistEnergyMarker = nullptr;

	cocos2d::ui::ImageView *retreatPanel = nullptr;
	cocos2d::Node *retreatEnergyMarker = nullptr;
	void updateRetreatEnergySpritePanel();

	/// Getters and Setters
	bool isEmpty() const { return !data; }
	bool isBasic() const;
	virtual Type getType() const { return Type::Pet; }

	virtual void setFlip(bool flip, bool anim = false) override;
private:
	static const std::string EMPTY_PET_IMG;
};


///////////////////////


class EnergyCard : public BSCard {
public:
	static EnergyCard* createWithData(const std::shared_ptr<const CardData> &data);
	//static EnergyCard* create(const PetCardData *data);
	static EnergyCard* create();

	virtual ~EnergyCard();

	virtual bool init() override;
	virtual bool initWithData(const std::shared_ptr<const CardData> &data) override;
	virtual void returnToPool();

	std::shared_ptr<const EnergyCardData> data;

	cocos2d::Node *cardNode = nullptr;
	cocos2d::ui::Text *nameText = nullptr;
	cocos2d::ui::ImageView *image = nullptr;
	cocos2d::ui::Layout *cardBack = nullptr;

	virtual std::shared_ptr<const CardData> getData() const override;
	virtual Type getType() const { return Type::Energy; }

	virtual void setNormalSize() override;

	virtual void setFlip(bool flip, bool anim = false) override;
};

BATTLE_SCENE_NS_END