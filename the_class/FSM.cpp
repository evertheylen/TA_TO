
#include "FSM.tcc"

// XML_Casts here. (and in header)
template <>
char XML_Cast<char>(std::string s) {
	return s.at(0);
}
