#include <vector>
#include "conv.h"

// Returns which bits are on in the integer a                                                                                                                                                                                              
std::vector<int> getOnLocations(int a) {
  std::vector<int> result;
  int place = 0;
  while (a != 0) {
    if (a & 1) {
      result.push_back(place);
    }
    ++place;
    a >>= 1;
  }
  return result;
}


