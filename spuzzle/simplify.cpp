#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <ctime>
#include <list>
#include <set>
using namespace std;

#include "simplify.hpp"



void simplify(KGraph &graph, int simplicity) {
    srand(time(NULL));

    set<int> unfinished;
    for (int i = 0; i < simplicity; i++) {
        graph.print(KState{graph.mapSize, 0}, 0);

        unfinished.clear();
        for (int j = 1; j < graph.size(); j++) {
            KMap &map = graph.get(0, j);
            if (map.empty()) {
                map = KMap{graph.mapSize, F};
                continue;
            } else {
                unfinished.insert(j);
            }
        }

        // Pick two rooms to simplify. TODO
        int a = -1, b = -1, comparison = 0;
        for (set<int>::iterator it = unfinished.begin(); it != unfinished.end(); it++) {
            set<int>::iterator jt = it;
            for (jt++; jt != unfinished.end(); jt++) {
                if (graph.get(*it, *jt).empty()) {
                    KMap ai = graph.get(0, *it);
                    KMap bi = graph.get(0, *jt);

                    KMap aDiff = ai & !(KMap)bi.boundingBox();
                    KMap bDiff = bi & !(KMap)ai.boundingBox();

                    int aDiffTerm;
                    int bDiffTerm;

                    if (aDiff.empty()) {
                        aDiffTerm = 0;
                    } else {
                        aDiffTerm = aDiff.boundingBox().termSize();
                    }

                    if (bDiff.empty()) {
                        bDiffTerm = 0;
                    } else {
                        bDiffTerm = bDiff.boundingBox().termSize();
                    }

                    int comp = aDiffTerm + bDiffTerm;
                    if (comp > comparison) {
                        a = *it;
                        b = *jt;
                        comparison = comp;
                    }
                }
            }
        }

        // Simplify the pair of rooms
        if (a == -1 || b == -1) {
            comparison = 1 << graph.mapSize;
            for (set<int>::iterator it = unfinished.begin(); it != unfinished.end(); it++) {
                KMap map = graph.get(0, *it);
                list<KBox> boxes = map.solve();
                if (boxes.size() > 1 && boxes.size() < comparison) {
                    a = *it;
                    comparison = boxes.size();
                }
            }
            if (a != -1) {
                breakPair(graph, 0, a);
            } else {
                break;
            }
        } else {
            simplifyPair(graph, 0, a, 0, b);
        }
    }
}



void simplifyPair(KGraph &graph, int iA0, int iA1, int iB0, int iB1) {
    cout << "Simplifying Pair " << iA0 << "-" << iA1 << " " << iB0 << "-" << iB1 << endl;
    if (iA0 == iA1 || iB0 == iB1) {
        cerr << "Cannot simplify a branch between the same node" << endl;
        exit(EXIT_FAILURE);
    }

    // Gather preexisting states
    KMap A0 = graph.fromAtoB(0, iA0);
    KMap A1 = graph.fromAtoB(0, iA1);
    KMap B0 = graph.fromAtoB(0, iB0);
    KMap B1 = graph.fromAtoB(0, iB1);

    // Generate new rooms
    int iAM = graph.add(KMap{graph.mapSize, F});
    int iBM = graph.add(KMap{graph.mapSize, F});

    // Swap roles of iA1 and iAM (and the B's too)
    graph.get(iA0, iA1) = KMap{graph.mapSize, F};
    graph.get(iB0, iB1) = KMap{graph.mapSize, F};

    // Grab the doors to set
    KMap &a0 = graph.get(iA0, iAM);
    KMap &a1 = graph.get(iAM, iA1);

    KMap &b0 = graph.get(iB0, iBM);
    KMap &b1 = graph.get(iBM, iB1);

    KMap &m = graph.get(iAM, iBM);


    /* Generate new passages TODO */
    // Clear out passages temporarily
    a0 = KMap{graph.mapSize, F};
    a1 = KMap{graph.mapSize, F};
    b0 = KMap{graph.mapSize, F};
    b1 = KMap{graph.mapSize, F};
    m = KMap{graph.mapSize, F};

    // Set branches to T, X, or F depending on accessibility now
    // "If A0 was specifically open and now might not be open, open a0"
    a0 = A0.exact() & (!graph.fromAtoB(0, iA0)).maybe();
    a1 = A1.exact() & (!graph.fromAtoB(0, iA1)).maybe();
    b0 = B0.exact() & (!graph.fromAtoB(0, iB0)).maybe();
    b1 = B1.exact() & (!graph.fromAtoB(0, iB1)).maybe();
}



void breakPair(KGraph &graph, int a, int b) {
    cout << "Breaking pair " << a << " " << b << endl;
    KMap ab = graph.get(a, b);
    graph.get(a, b) = KMap{graph.mapSize, F};

    list<KBox> sol = ab.solve();
    for (list<KBox>::iterator it = sol.begin(); it != sol.end(); it++) {
        int c = graph.add(KMap{graph.mapSize, F});
        graph.get(a, c) = graph.get(c, b) = (KMap)(*it);
    }
}
