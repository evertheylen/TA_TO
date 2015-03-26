#include "FSM.tcc"
#include <string>
#include <set>

template <>
std::set<std::string> irritating_cast<const char*, std::set<std::string>>(const char* f);
