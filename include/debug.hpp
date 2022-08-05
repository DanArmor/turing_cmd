#define DEBUG
#ifdef DEBUG
#ifndef _INC_DEBUG_HPP
#define _INC_DEBUG_HPP
#include <string>

extern std::ofstream DEBUG_output;

#define DEBUG_write(str) {DEBUG_output << str << "\n";}


#endif
#endif