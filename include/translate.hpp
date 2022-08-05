#ifndef _INC_TRANSLATE_HPP
#define _INC_TRANSLATE_HPP

#include "debug.hpp"

#include <map>
#include <vector>
#include <string>
#include <fstream>

enum LANG{
    L_EN,
    L_RU
};

extern std::map<std::string, std::vector<std::string>> LANGUAGE;
extern std::string TRANSLATE_EMPTY_STR;

std::string &locale_getText(std::string const &key, LANG lang);

#endif
