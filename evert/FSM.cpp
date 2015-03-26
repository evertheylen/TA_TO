#include "FSM.tcc"
#include "FSM.h"
#include <string>
#include <set>


template <>
std::set<std::string> irritating_cast<const char*, std::set<std::string>>(const char* f) {
	return std::set<std::string>();
}
