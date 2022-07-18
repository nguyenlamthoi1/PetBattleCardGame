#include "LangSys.h"
#include "cocos2d.h"
#include <sstream>

USING_NS_CC;
using namespace std;

LangSys::LangSys(){}
LangSys::~LangSys() {
	CCLOG("LangSys::dtor called");
};

bool LangSys::load(const string &path) {
	localizedStrings.clear();

	string contents = FileUtils::getInstance()->getStringFromFile(path); // Lay data tu file
	if (contents.empty()) // load that bai
		return false;
	
	istringstream fileStringStream(contents); // Tao 1  istringstream de su dung getLine function
	string line;

	// Get file contents line by line
	while (getline(fileStringStream, line))
	{
		auto validPos = line.find('=');
		if (validPos == string::npos)
			continue;

		// key & string
		std::string keyStr = line.substr(0, validPos - 1);

		// fix: remove special chars
		// line.substr(validPos + 1, line.size() - 1);
		validPos += 3;
		size_t indexValidPos = 1;

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//		indexValidPos = 1;
//#else
//		indexValidPos = 1;
//#endif

		string subStr = line.substr(validPos, line.size() - validPos - indexValidPos);
		size_t index = 0;
		while (subStr.find("\\n", index) != string::npos)
		{
			index = subStr.find("\\n", index);

			subStr.replace(index, 2, "\n ");
		}
		localizedStrings.insert(pair<string, string>(keyStr, subStr));
	}
	return true;
}
string LangSys::getString(const std::string &key) const {
	auto it = localizedStrings.find(key);
	if (it != localizedStrings.end())
		return it->second;
	return key;
}

void LangSys::clear() {
	localizedStrings.clear();
}
