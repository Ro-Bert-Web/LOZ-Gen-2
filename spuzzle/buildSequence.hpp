#ifndef BUILD_SEQUENCE_HPP
#define BUILD_SEQUENCE_HPP

#include "../kgraph/kgraph.hpp"
#include "nperm.hpp"

void buildSequence(KGraph&, int, int, int, int);
bool chooseSwitch(KGraph&, int, int, int, set<int>&, KState&, int);
bool chooseKey(KGraph&, int, int, int, set<int>&, KState&, int);
bool chooseCataclysm(KGraph&, int, int, int, set<int>&, KState&, int);

#endif
