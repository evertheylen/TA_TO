
#include "CompactDFA.h"

int CompactDFA::delta(int s, char c) {
	return d_data[s][c];
}

bool CompactDFA::is_dead(int s) {
	return Dead[s];
}

bool CompactDFA::is_final(int s) {
	return Final[s];
}
