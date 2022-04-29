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
            /*} else if (map.solve().size() == 1) {
                map.resolve();
                continue;*/
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
                breakPair(graph, a);
            } else {
                break;
            }
        } else {
            simplifyPair(graph, a, b);
        }
    }

    for (int i = 1; i < graph.size(); i++)
        breakPair(graph, i);
}



void simplifyPair(KGraph &graph, int iA, int iB) {
    cout << "Simplifying Pair " << iA << " " << iB << endl;
    if (iA == iB) {
        cerr << "Cannot simplify a branch between the same node" << endl;
        exit(EXIT_FAILURE);
    }

    // Gather preexisting states
    KMap A = graph.fromAtoB(0, iA);
    KMap B = graph.fromAtoB(0, iB);

    // Generate new rooms
    int iAM = graph.add(KMap{graph.mapSize, F});
    int iBM = graph.add(KMap{graph.mapSize, F});

    // Swap roles of iA1 and iAM (and the B's too)
    graph.get(0, iA) = KMap{graph.mapSize, F};
    graph.get(0, iB) = KMap{graph.mapSize, F};

    // Grab the doors to set
    KMap &a0 = graph.get(0, iAM);
    KMap &a1 = graph.get(iAM, iA);

    KMap &b0 = graph.get(0, iBM);
    KMap &b1 = graph.get(iBM, iB);

    KMap &m = graph.get(iAM, iBM);


    /* Generate new passages TODO */
    // Clear out passages temporarily
    a0 = KMap{graph.mapSize, F};
    a1 = KMap{graph.mapSize, F};
    b0 = KMap{graph.mapSize, F};
    b1 = KMap{graph.mapSize, F};
    m = KMap{graph.mapSize, F};

    // Set branches to T, X, or F depending on accessibility now
    // "If A was specifically open and now might not be open, open a1"
    a1 = A.exact() & (!graph.fromAtoB(0, iA)).maybe();
    a1 = a1.empty() ? a1.exact() : a1.boundingBox();

    b1 = B.exact() & (!graph.fromAtoB(0, iB)).maybe();
    b1 = b1.empty() ? b1.exact() : b1.boundingBox();

    // "If both a1 and b1 boxes but either A or B should not be open, m should be closed"
    m = (!(a1 & b1 & ((!A).exact() | (!B).exact())) & KMap{graph.mapSize, X}).largestBox();

    /* Calculate optimal a0 and b0 */
    // Start by finding their masks (when they can and cannot be on)
    // Set masks to what needs to be open
    KMap aMask = A.exact() & (!graph.fromAtoB(0, iA)).maybe();
    KMap bMask = B.exact() & (!graph.fromAtoB(0, iB)).maybe();

    // Find when a0 cannot be open
    a0 = KMap{graph.mapSize, T};
    a0 = ((!A).exact() & graph.fromAtoB(0, iA).maybe()) |
        ((!B).exact() & graph.fromAtoB(0, iB).maybe());
    if (!(a0 & aMask).empty()) {
        cerr << "A0 must not be open and must be open. Probably m is wrong?" << endl;
        exit(EXIT_FAILURE);
    }
    aMask = aMask | KMap{graph.mapSize, X} & !a0;
    a0 = KMap{graph.mapSize, F};

    // Find when b0 cannot be open
    b0 = KMap{graph.mapSize, T};
    b0 = ((!A).exact() & graph.fromAtoB(0, iA).maybe()) |
        ((!B).exact() & graph.fromAtoB(0, iB).maybe());
    if (!(b0 & bMask).empty()) {
        cerr << "B0 must not be open and must be open. Probably m is wrong?" << endl;
        exit(EXIT_FAILURE);
    }
    bMask = bMask | KMap{graph.mapSize, X} & !b0;
    b0 = KMap{graph.mapSize, F};


    // Split the masks between each other for optimal distribution
    // "Every spot in a0 that cannot be covered by b0, along with all a0's X's"
    a0 = aMask & (!bMask).exact() | aMask.maybe() & KMap{graph.mapSize, X};
    b0 = bMask & (!aMask).exact() | bMask.maybe() & KMap{graph.mapSize, X};

    aMask &= !a0.exact() | KMap{graph.mapSize, X};
    bMask &= !b0.exact() | KMap{graph.mapSize, X};

    // Three cases for points that could be covered by both:
    //  in a0's solution but not b0's solution (goes to a0 and b0 gets an X)
    //  in both (doesn't matter)
    //  in neither (pick the one that would make them smallest [somehow])

    // In one but not the other
    KMap crit = (aMask | bMask) & a0.resolve() & !b0.resolve();
    a0 |= crit;
    b0 &= !(crit & KMap{graph.mapSize, X});

    crit = (aMask | bMask) & b0.resolve() & !a0.resolve();
    b0 |= crit;
    a0 &= !(crit & KMap{graph.mapSize, X});

    aMask &= !a0.exact() | KMap{graph.mapSize, X};
    bMask &= !b0.exact() | KMap{graph.mapSize, X};

    // In both
    crit = (aMask | bMask) & a0.resolve() & b0.resolve();
    // Encourage passthrough logic (if a0 requires it but b0 can have it, use b0)
    a0 |= crit & bMask.exact() & !aMask.exact();
    b0 |= crit & aMask.exact() & !bMask.exact();

    aMask &= !a0.exact() | KMap{graph.mapSize, X};
    bMask &= !b0.exact() | KMap{graph.mapSize, X};


    a0 |= aMask.exact();
    b0 |= bMask.exact();


    // set a0, b0, and m
    // Goals:
    //  get a1, b1, and m to be solid boxes
    //  minimize the complexity of a0 and b0
    //  what is complexity?
    //   amount of terms in solution? ***
    //   size of bounding box?
}



void breakPair(KGraph &graph, int a) {
    cout << "Breaking pair " << a << endl;
    KMap A = graph.get(0, a);
    list<KBox> sol = A.solve();
    if (sol.size() > 1) {
        graph.get(0, a) = KMap{graph.mapSize, F};
        for (list<KBox>::iterator it = sol.begin(); it != sol.end(); it++) {
            int m = graph.add(KMap{graph.mapSize, F});
            graph.get(0, m) = graph.get(m, a) = (KMap)(*it);
        }
    } else {
        graph.get(0, a) = graph.get(0, a).resolve();
    }
}
