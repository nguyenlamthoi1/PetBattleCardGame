#pragma once

#include "../../GameDefine.h"
#include "data/CardData.h"

#include <string>
#include <memory>

NS_GAME_BEGIN

/*
	- Card Data Wrapper
	- Abstract Base Class cho cac loai card(PetCard, EnergyCard,..)
*/
class Card {
public:
	using CardId = std::string;
	using CDataPtr = std::shared_ptr<const CardData>;
	using CardShPtr = std::shared_ptr<Card>;

	static CardShPtr createShPtr(const CardId &id);

	enum class Type {
		None,
		Pet,
		Energy,
		Trainer
	};

	/// Ctors and Dtor
	Card(Type type, CardId id);
	virtual ~Card();

	/// Getters, Setters & Checkers
	virtual CDataPtr getData() const = 0;

	virtual CardId getId() const;

	virtual Type getType() const;
	virtual bool isType(Type type) const;

	virtual bool isValid() const; // Kiem tra card object co the su dung khong

protected:
	Type type = Type::None;
	CardId id;
};


class PetCard : public Card {
public:
	using CPetDataPtr = std::shared_ptr<const PetCardData>;

	/// Ctors and Dtor
	PetCard(Type type, const CardId &id);
	PetCard(Type type, const CardId &id, const CPetDataPtr &data);
	virtual ~PetCard();

	/// Getters, Setters & Checkers
	virtual void setData(const CDataPtr &dtaPtr);
	virtual CDataPtr getData() const override;
	virtual CPetDataPtr getPetData() const;

	virtual bool isValid() const override;

protected:
	CPetDataPtr petData;
};

class EnergyCard : public Card {
public:
	using CEnergyDataPtr = std::shared_ptr<const EnergyCardData>;

	/// Ctors and Dtor
	EnergyCard(Type type, const CardId &id);
	EnergyCard(Type type, const CardId &id, const CEnergyDataPtr &data);
	virtual ~EnergyCard();

	/// Getters, Setters & Checkers
	virtual void setData(const CDataPtr &dtaPtr);
	virtual CDataPtr getData() const override;
	virtual CEnergyDataPtr getEnergyData() const;

	virtual bool isValid() const override;

protected:
	CEnergyDataPtr energyData;
};

NS_GAME_END