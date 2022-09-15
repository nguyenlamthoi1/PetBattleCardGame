#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__

#include <string>

namespace EVENT_CUSTOM{
	const std::string RES_LOADING_FAILED = "RES_LOADING_FAILED"; // Event Id
	struct EC_LoadingFailedData { // Event data tra ve

	};
	const std::string RES_LOADING_FINISHED = "RES_LOADING_FINISHED";
	struct EC_LoadingFinishedData {
		unsigned int loaded;
		unsigned int total;
		
	};
	const std::string RES_LOADING_STEP_FINISHED = "RES_LOADING_STEP_FINISHED";
	struct EC_LoadStepFinishedData {
		std::string fName;
		bool suc;
		unsigned int cur;
		unsigned int total;
	};


};

namespace CardDefine {

	using EnergyType = std::string;
	namespace Energy {
		const EnergyType None = "";
		const EnergyType Normal = "Normal";
		const EnergyType Fire = "Fire";
		const EnergyType Water = "Water";
		const EnergyType Grass = "Grass";
		const EnergyType Fighting = "Fighting";
		const EnergyType Psychic = "Psychic";
		const EnergyType Electric = "Electric";
		const EnergyType Dark = "Dark";
		const EnergyType Dragon = "Dragon";
		const EnergyType Steel = "Steel";
	}

	namespace PetType {
		const std::string None = "";
		const std::string Normal = "Normal";
		const std::string Fire = "Fire";
		const std::string Water = "Water";
		const std::string Grass = "Grass";
		const std::string Fighting = "Fighting";
		const std::string Psychic = "Psychic";
		const std::string Electric = "Electric";
		const std::string Dark = "Dark";
		const std::string Dragon = "Dragon";
		const std::string Steel = "Steel";
	}
}

namespace GConfig { // Game Config
	constexpr unsigned int PRIZE_CARDS_NUM_PER_PLAYER = 6;
	constexpr unsigned int CARD_NUM_IN_DECK_MAX = 60;
	constexpr unsigned int DEFAULT_BENCH_CAPACITY = 5;
	constexpr unsigned int DRAW_NUM_PER_TURN = 1;
	constexpr unsigned int DRAW_NUM_ON_START = 7;

	/// Player actions
	constexpr unsigned int ATTACH_ENERGY_TURN_LIMIT = 1; // Gioi han so lan attach energy moi luot
	constexpr unsigned int EVOLVE_PET_TURN_LIMIT = UINT_MAX; // Gioi han so lan attach energy card tu tay len pet moi luot
	constexpr unsigned int PLAY_BASIC_PET_TURN_LIMIT = UINT_MAX; // Gioi han so lan choi basic pet card moi luot
	constexpr unsigned int RETREAT_PET_TURN_LIMIT = 1; // Gioi han so lan retreat active pet moi luot
	constexpr unsigned int PLAY_SPT_TURN_LIMIT = 1; // Gioi han so lan choi supporter card moi luot
	constexpr unsigned int PLAY_ITEM_TURN_LIMIT = UINT_MAX; // Gioi han so lan play item card moi luot
	constexpr unsigned int USE_PET_ABILITY_TURN_LIMIT = UINT_MAX; // Gioi han so lan play item card moi luot
}

/// Global variables
const std::string PLAYER_ID = "Player";

#endif __COMMON_DEFINE_H__