#include <iostream>
#include <fstream>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <list>
#include <set>
using namespace std;

#include "spuzzle.hpp"
#include "buildSequence.hpp"
#include "simplify.hpp"



KGraph spuzzle(int numSwitches, int numKeys, int numCataclysms, int pressure, int simplicity) {
    int numTriggers = numSwitches + numKeys + numCataclysms;

    // Create the initial graph framework
    cout << "Generating initial graph framework with " << numTriggers << " triggers and " << numTriggers + 1 << " rooms..." << endl;
    KGraph graph{numTriggers};
    for (int i = 1; i <= numTriggers; i++) {
        cout << "Adding one trigger room..." << endl;
        graph.add(KMap{numTriggers, F});
        KMap map{numTriggers, X};

        if (i > numSwitches) {
            if (i > numSwitches + numKeys) {
                cout << "   turning it into a cataclysm" << endl;
            } else {
                cout << "   turning it into a key" << endl;
            }
            KBox box{numTriggers};
            box[i - 1] = T;
            map &= box;
        }
        graph.set(0, i, map);
    }

    graph.add(KMap{numTriggers, F});

    // Building the paths
    buildSequence(graph, numSwitches, numKeys, numCataclysms, pressure);

    // Simplify the dungeon
    KGraph duplicate = graph;

    simplify(graph, simplicity);

    bool error = false;
    for (int i = 1; i < duplicate.size(); i++) {
        KMap before = duplicate.fromAtoB(0, i);
        KMap after = graph.fromAtoB(0, i);

        // If it was specifically open before, and is now maybe not open
        //  or it was specifically not open and is now maybe open
        if (!(( (before.exact() & (!after).maybe()) |
                ((!before).exact() & after.maybe())).empty())) {
            cerr << "Graphs are different after simplification: " << i << endl;;

            cout << "Before: ";
            for (int i = 0; i < (1 << graph.mapSize); i++) {
                if (before[i] == T) {
                    cout << "T";
                } else if (before[i] == X) {
                    cout << "X";
                } else {
                    cout << "F";
                }
            }
            list<KBox> sol = before.solve();
            for (list<KBox>::iterator it = sol.begin(); it != sol.end(); it++)
                cout << " " << it->show();
            cout << endl;

            cout << "After:  ";
            for (int i = 0; i < (1 << graph.mapSize); i++) {
                if (after[i] == T) {
                    cout << "T";
                } else if (after[i] == X) {
                    cout << "X";
                } else {
                    cout << "F";
                }
            }
            sol = after.solve();
            for (list<KBox>::iterator it = sol.begin(); it != sol.end(); it++)
                cout << " " << it->show();
            cout << endl;

            error = true;
        }
    }
    if (error)
        exit(EXIT_FAILURE);

    return graph;
}
