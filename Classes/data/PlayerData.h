#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#include <string>
#include <unordered_map>
#include <vector>

class CardData;
class DataManager;

class PlayerData {
public:
	friend class DataManager;

	enum class CardType {
		Pokemon,
		Energy,
		Supporter,
		Item
	};

	using CardId = std::string;
	using CardNum = unsigned int;
	using DeckList = std::vector<std::shared_ptr<const CardData>>;

	using DeckMap = std::unordered_map<CardId, CardNum>;

	using CardList = std::unordered_map<CardId, CardData>;
	
	static std::shared_ptr<PlayerData> createPseudo(bool isPlayer);

	PlayerData();
	virtual ~PlayerData();
	bool loadDataFromFile(const std::string &file);
	DeckList getCurDeck() const;
private:
	
	std::vector<DeckList> decks;

	std::vector<DeckMap> deckList;
	size_t curDeck = 0;

	std::unordered_map<CardId, CardNum> ownedCards;

	std::unordered_map<CardType, CardList> cards;
};

#endif // __PLAYER_DATA_H__