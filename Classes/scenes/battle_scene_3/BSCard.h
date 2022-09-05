#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"

#include "BSDefine.h"
#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

class CardData;
class PetCardData;
class EnergyCardData;

BATTLE_SCENE_NS_BEG

class BSCard : public cocos2d::ui::Layout {
public:
	using DragHandler = std::function<void(cocos2d::Node*, cocos2d::Node*)> ;

	static const cocos2d::Size ORIGIN_CARD_SIZE;
	static const float CARD_SCALE_DOWN;
	static cocos2d::Size CARD_SIZE;

	static BSCard* createWithData(const std::shared_ptr<const CardData> &data);
	virtual std::shared_ptr<const CardData> getData() = 0;
	virtual void setNormalSize() = 0;
	virtual void returnToPool(){}

	// Getters and Setters
	virtual bool isFlippedDown() const { return flippedDown; }
	virtual void setFlip(bool flip, bool anim = false) = 0; // Flip card ap xuong khi card dang ngua, va nguoc lai
	virtual float getFliptime() const { return flipTime; }

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

protected:
	BSCard();
	virtual ~BSCard();
	virtual bool init() override = 0;
	virtual bool initWithData(const std::shared_ptr<const CardData> &data) = 0;
	virtual bool onTouchHold();

	DragHandler dragBeg = nullptr;
	DragHandler dragEnd = nullptr;
	DragHandler dragIn = nullptr;
	DragHandler dragOut = nullptr;

	bool flippedDown = false;
	float flipTime = 0.0f;
};

class PetCard : public BSCard {
public:

	static PetCard* createWithData(const std::shared_ptr<const CardData> &data);
	//static PetCard* create(const PetCardData *data);
	static PetCard* create();

	virtual bool init() override;
	virtual bool initWithData(const std::shared_ptr<const CardData> &data) override;
	virtual void returnToPool();
	virtual std::shared_ptr<const CardData> getData() override;
	virtual void setNormalSize() override;

	std::shared_ptr<const PetCardData> data;

	cocos2d::Node *cardNode = nullptr;
	cocos2d::ui::Text *nameText = nullptr;
	cocos2d::ui::Text *hpText = nullptr;
	cocos2d::ui::ImageView *petImg = nullptr;
	cocos2d::ui::ImageView *outerImg = nullptr;
	cocos2d::ui::Layout *cardBack = nullptr;

	cocos2d::ui::Text *evText = nullptr;
	cocos2d::ui::ImageView *evArrow = nullptr;
	cocos2d::ui::Text *evFromText = nullptr;
	cocos2d::ui::ImageView *evFromImg = nullptr;

	/// Getters and Setters
	bool isEmpty() const { return !data; }
	bool isBasic() const;

	virtual void setFlip(bool flip, bool anim = false) override;
private:
	static const std::string EMPTY_PET_IMG;
};

class EnergyCard : public BSCard {
public:
	static EnergyCard* createWithData(const std::shared_ptr<const CardData> &data);
	//static EnergyCard* create(const PetCardData *data);
	static EnergyCard* create();

	virtual bool init() override;
	virtual bool initWithData(const std::shared_ptr<const CardData> &data) override;
	virtual void returnToPool();

	std::shared_ptr<const EnergyCardData> data;

	cocos2d::Node *cardNode = nullptr;
	cocos2d::ui::Text *nameText = nullptr;
	cocos2d::ui::ImageView *image = nullptr;
	cocos2d::ui::Layout *cardBack = nullptr;

	virtual std::shared_ptr<const CardData> getData() override;
	virtual void setNormalSize() override;

	virtual void setFlip(bool flip, bool anim = false) override;
};

BATTLE_SCENE_NS_END