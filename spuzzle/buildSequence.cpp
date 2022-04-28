#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstddef>
#include <ctime>
#include <list>
#include <set>
using namespace std;

#include "buildSequence.hpp"

ofstream file;
string attributes;



void buildSequence(KGraph &graph, int numSwitches, int numKeys, int numCataclysms, int pressure) {
    srand(time(NULL));

    file.open("state_diagram.dot");
    file << "graph \"dungeon\" {" << endl;

    int numTriggers = numSwitches + numKeys + numCataclysms;

    cout << endl << "Generating solution path..." << endl;

    int fails = 0;
    set<int> visited;
    int remainingKeys = numKeys;
    int remainingCataclysms = numCataclysms;
    KState state{numTriggers};

    // Generate solution path
    for (int i = 0; i < numKeys + numCataclysms; i++) {
        for (int j = 0; j < (2 * numSwitches - 1) * pressure / 100; j++) {
            // Generate random permutation of all the switches
            list<int> trials = nperm(rand() % fact(numSwitches), numSwitches);

            // Try one after the other until one works
            list<int>::iterator it;
            for (it = trials.begin(); it != trials.end(); it++)
                if (chooseSwitch(graph, numSwitches, numKeys, numCataclysms, visited, state, *it))
                    break;
            if (it == trials.end())
                break;
        }

        int choice = rand() % (remainingKeys + remainingCataclysms);
        if (choice < remainingKeys) {
            chooseKey(graph, numSwitches, numKeys, numCataclysms, visited, state, --remainingKeys);
        } else {
            chooseCataclysm(graph, numSwitches, numKeys, numCataclysms, visited, state, --remainingCataclysms);
        }
    }

    for (int i = 0; i < (2 * numSwitches - 1) * pressure / 100; i++) {
        // Generate random permutation of all the switches
        list<int> trials = nperm(rand() % fact(numSwitches), numSwitches);

        // Try one after the other until one works
        list<int>::iterator it;
        for (it = trials.begin(); it != trials.end(); it++)
            if (chooseSwitch(graph, numSwitches, numKeys, numCataclysms, visited, state, *it))
                break;
        if (it == trials.end())
            break;
    }

    // Declare exit state
    graph.get(0, numTriggers + 1)[state] = T;


    attributes = " style=dotted";

    // Generate side paths
    cout << endl << "Generating side paths..." << endl;
    set<int> unfinished = visited;
    unfinished.erase(unfinished.find(state.toCell()));
    while (unfinished.size() > 0) {
        int choice = rand() % unfinished.size();

        set<int>::iterator it = unfinished.begin();
        while (choice > 0) {
            it++;
            choice--;
        }
        state = KState{numTriggers, *it};
        cout << "Branching state " << state.toCell() << "..." << endl;

        // Generate random permutation of all the switches
        list<int> trials = nperm(rand() % fact(numSwitches), numSwitches);

        // Try one after the other until one works
        list<int>::iterator jt;
        for (jt = trials.begin(); jt != trials.end(); jt++)
            if (chooseSwitch(graph, numSwitches, numKeys, numCataclysms, visited, state, *jt))
                break;
        if (jt == trials.end()) {
            unfinished.erase(it);
        } else {
            unfinished.insert(state.toCell());
        }
        cout << endl;
    }

    file << "}" << endl;
    file.close();
}



bool chooseSwitch(KGraph &graph, int numSwitches, int numKeys, int numCataclysms, set<int> &visited, KState &state, int swtch) {
    visited.insert(state.toCell());
    bool success;
    int numTriggers = numSwitches + numKeys + numCataclysms;

    int index = swtch;
    int room = index + 1;
    cout << "Switch " << swtch << " in room " << room << " => ";

    KMap &map = graph.get(0, room);

    // If going into new state
    KState newState{numTriggers};
    newState = state;
    newState[index] = !(newState[index]);
    if (visited.count(newState.toCell()) == 0) {
        file << "   " << state.toCell() << " -- " << newState.toCell() << " [label=\"S" << swtch << "\"" << attributes << "];" << endl;

        cout << "State " << state.toCell() << " to " << newState.toCell() << endl;
        map[state] = T;
        map[newState] = T;
        state = newState;
        success = true;
        visited.insert(state.toCell());
    }
    else {
        cout << "X" << endl;
        if (map[state] != T) {
            map[state] = F;
            map[newState] = F;
        }
        success = false;
    }

    return success;
}



bool chooseKey(KGraph &graph, int numSwitches, int numKeys, int numCataclysms, set<int> &visited, KState &state, int key) {
    visited.insert(state.toCell());
    bool success = true;
    int numTriggers = numSwitches + numKeys + numCataclysms;

    int index = numSwitches + key;
    int room = index + 1;
    cout << "Key " << key << " in room " << room << " => ";

    KMap &map = graph.get(0, room);

    // Check for unexpected error of used key
    if (state[index] == true) {
        cerr << "Wait. How'd a key get set to true already?" << endl;
        exit(EXIT_FAILURE);
    }

    KState newState = state;
    newState[index] = true;
    cout << "State " << state.toCell() << " to " << newState.toCell() << endl;

    file << "   " << state.toCell() << " -- " << newState.toCell() << " [label=\"K" << key << "\"" << attributes << "];" << endl;

    map[state] = T;
    map[newState] = T;
    state = newState;

    return success;
}



bool chooseCataclysm(KGraph &graph, int numSwitches, int numKeys, int numCataclysms, set<int> &visited, KState &state, int cataclysm) {
    visited.insert(state.toCell());
    bool success = true;
    int numTriggers = numSwitches + numKeys + numCataclysms;

    int index = numSwitches + numKeys + cataclysm;
    int room = index + 1;
    cout << "Cataclysm " << cataclysm << " in room " << room << " => ";

    KMap &map = graph.get(0, room);

    // Check for unexpected error of used cataclysm
    if (state[index] == true) {
        cerr << "Wait. How'd a cataclysm get set to true already?" << endl;
        exit(EXIT_FAILURE);
    }

    KState newState = state;
    newState[index] = true;
    cout << "State " << state.toCell() << " to " << newState.toCell() << endl;

    file << "   " << state.toCell() << " -- " << newState.toCell() << " [label=\"C" << cataclysm << "\"" << attributes << "];" << endl;

    map[state] = T;
    map[newState] = T;
    state = newState;

    return success;
}
