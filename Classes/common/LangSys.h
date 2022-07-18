#ifndef __LANG_SYS_H__
#define __LANG_SYS_H__

#include <unordered_map>
#include <string>

class LangSys // copied from LanguageMgr
{
public:
	LangSys();
	~LangSys();
	bool load(const std::string& path);
	std::string getString(const std::string& key) const;
	void clear();
private:
	std::unordered_map<std::string, std::string> localizedStrings;
};

#endif // __LANG_SYS_H__