#pragma once

#include "../GameDefine.h"

#include <deque>
#include <vector>
#include <array>

class CardData;

NS_GAME_BEGIN

class GameState;
class Card;

class PrizePile {
public:
	friend class GetPrizeCards;

	using CardVector = std::array<std::shared_ptr<Card>, GConfig::PRIZE_CARDS_NUM_PER_PLAYER>;
	using CardArray = std::array<std::shared_ptr<Card>, GConfig::PRIZE_CARDS_NUM_PER_PLAYER>;

	using PrizeIdx = unsigned int;
	using PrizePtr = std::shared_ptr<PrizePile>;

	static PrizePtr createShPtr(const PlayerIdType &id);

	PrizePile(const PlayerIdType &id);
	virtual ~PrizePile();
	bool init();

	std::vector<unsigned int> getFirstSelectables(unsigned int num) const;

	bool canGetCardsAt(const std::vector<unsigned int> &idxVec) const;
	void saveSelectedCardsAt(const std::vector<unsigned int> &idxVec);

	void pushCards(const std::vector<std::shared_ptr<Card>> &pushedCards);
	void pushCard(const std::shared_ptr<Card> &pushedCard);

	void popCards(const std::vector<PrizeIdx> &idxVec, const std::vector<std::shared_ptr<Card>> &retVec);
	std::vector<std::shared_ptr<Card>> popCards(const std::vector<PrizeIdx> &idxVec);

	
	void popSelectedCards(std::vector<std::shared_ptr<Card>> &retVec);
	std::vector<std::shared_ptr<Card>> popSelectedCards();

	void clear();
	bool empty() const;

	PrizePtr clone() const;
	
protected:
	PlayerIdType pid;
	CardVector cardVec = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	std::vector<unsigned int> selected;
};

NS_GAME_END