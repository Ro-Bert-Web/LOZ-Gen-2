#ifndef SIMPLIFY_HPP
#define SIMPLIFY_HPP

#include "../kgraph/kgraph.hpp"

void simplify(KGraph&, int);
void simplifyPair(KGraph&, int, int, int, int);
void breakPair(KGraph&, int, int);

#endif
