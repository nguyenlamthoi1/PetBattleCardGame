#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#include <string>
#include <unordered_map>
#include <vector>

class CardData;
class PlayerData {
public:
	enum class CardType {
		Pokemon,
		Energy,
		Supporter,
		Item
	};

	using CardId = std::string;
	using DeckList = std::vector<CardData*>;
	using CardList = std::unordered_map<CardId, CardData>;
	
	static PlayerData* createPseudo(bool isPlayer);

	PlayerData();
	virtual ~PlayerData();
	bool loadDataFromFile(const std::string &file);
	DeckList getCurDeck() const;
private:
	std::vector<DeckList> decks;
	size_t curDeck = 0;
	std::unordered_map<CardType, CardList> cards;
};

#endif // __PLAYER_DATA_H__