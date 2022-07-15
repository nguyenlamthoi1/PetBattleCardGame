#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__

#include <string>

namespace EVENT_CUSTOM{
	const std::string RES_LOADING_FAILED = "RES_LOADING_FAILED"; // Event Id
	struct EC_LoadingFailedData { // Event data tra ve

	};
	const std::string RES_LOADING_FINISHED = "RES_LOADING_FINISHED";
	struct EC_LoadingFinishedData {

	};
	const std::string RES_LOADING_STEP_FINISHED = "RES_LOADING_STEP_FINISHED";
	struct EC_LoadStepFinishedData {
		std::string fName;
		bool suc;
	};


};

#endif __COMMON_DEFINE_H__