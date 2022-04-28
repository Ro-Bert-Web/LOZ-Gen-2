#ifndef KGRAPH_HPP
#define KGRAPH_HPP

#include <vector>
#include <set>

#include "../karnaugh/kmap.hpp"

class KGraph {
    vector<KMap> edges;

    int pairToIndex(int, int);
    KMap fromAtoBHelper(int, int, set<int>, KMap);
public:
    const int mapSize;

    KGraph(int);

    int size();

    int add();
    int add(KMap);
    void set(int, int, KMap);
    KMap& get(int, int);

    KGraph& operator=(const KGraph&);

    KMap fromAtoB(int, int);

    void play(int, int, int);
    void print(KState, int);

#ifdef DEBUG
    static void test();
#endif
};

#endif
