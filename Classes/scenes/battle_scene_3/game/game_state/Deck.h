#pragma once

#include "../GameDefine.h"

#include <deque>
#include <vector>

class CardData;

NS_GAME_BEGIN

class Card;

class Deck {
public:
	using CardVector = std::vector<std::shared_ptr<Card>>;
	using DeckPtr = std::shared_ptr<Deck>;

	static DeckPtr createShPtr(const PlayerIdType &id);
	
	Deck(const PlayerIdType &id);
	virtual ~Deck();
	bool init();

	void pushCards(const CardVector &pushedCards);
	void pushCard(const std::shared_ptr<Card> &pushedCard);
	
	CardVector popTop(unsigned int num); // * Rut 1 so la bai o top deck, back cua cardVec duoc xem la top
	void popTop(unsigned int num, CardVector &v); // * Rut 1 so la bai o top deck, back cua cardVec duoc xem la top
	void popTop(unsigned int num, std::vector<std::string> &cidVec);
	void popTopWithABasicPet(unsigned int num, std::vector<std::string> &cidVec);
	void popTopWithABasicPet(unsigned int num, CardVector &cidVec);
	
	CardVector popBottom(unsigned int num); // * Rut 1 so la bai o bottom deck, back cua cardVec duoc xem la top
	void popBottom(unsigned int num, CardVector &v); // * Rut 1 so la bai o top deck, back cua cardVec duoc xem la top
	void popBottom(unsigned int num, std::vector<std::string> &cidVec);

	void shuffleDeck();

	void clear();
	bool isEmpty();

	DeckPtr clone() const;
private:

	PlayerIdType pid = PlayerIdInvalid; // Id cua player so huu Deck
	CardVector cardVec; // Vector cac la bai co trong Deck cua nguoi choi
};

NS_GAME_END