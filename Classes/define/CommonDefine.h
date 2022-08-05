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
	}
}

#endif __COMMON_DEFINE_H__