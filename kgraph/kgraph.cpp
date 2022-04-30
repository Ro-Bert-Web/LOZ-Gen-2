#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

#include "kgraph.hpp"



KGraph::KGraph(int mapSize): mapSize{mapSize} {
}



/* Setters and Getters */
int KGraph::pairToIndex(int a, int b) {
    int rslt;
    if (a == b) {
        rslt = -1;
    } else if (a < b) {
        rslt = pairToIndex(b, a);
    } else {
        int edge = a - 1;
        rslt = (edge * edge + edge) / 2 + b;
    }
    if (rslt >= edges.size())
        rslt = -1;
    return rslt;
}

int KGraph::size() {
    int rslt;
    for (rslt = 1; pairToIndex(rslt, 0) != -1; rslt++) {}
    return rslt;
}

int KGraph::add() {
    int newSize = size();
    edges.resize(edges.size() + newSize, {mapSize});
    return newSize;
}

int KGraph::add(KMap map) {
    int newSize = size();
    edges.resize(edges.size() + newSize, map);
    return newSize;
}

void KGraph::set(int a, int b, KMap cond) {
    int index = pairToIndex(a, b);
    if (index < edges.size() && index >= 0) {
        edges[index] = cond;
    } else {
        cerr << "KGraph: Index " << a << ", " << b << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
}

KMap& KGraph::get(int a, int b) {
    KMap *rslt;
    int index = pairToIndex(a, b);
    if (index < edges.size() && index >= 0) {
        rslt = &edges[index];
    } else {
        cerr << "KGraph: Index " << a << ", " << b << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
    return *rslt;
}



/* Assignment Operators */
KGraph& KGraph::operator=(const KGraph &b) {
    if (b.mapSize != mapSize) {
        cerr << "KGraph: cannot copy between KGraphs of different sizes" << endl;
        exit(EXIT_FAILURE);
    }
    edges = b.edges;
    return *this;
}



/* Search Algorithms */
KMap KGraph::fromAtoBHelper(int a, int b, std::set<int> searched, KMap map) {
    KMap rslt{mapSize};
    rslt = KMap{mapSize, F};
    if (a == b || map.zero()) {
        rslt = map;
    }
    else if (searched.count(a) == 0) {
        searched.insert(a);
        for (int i = 0; i < size(); i++)
            if (i != a)
                rslt |= fromAtoBHelper(i, b, searched, map & get(a, i));
    }
    return rslt;
}

KMap KGraph::fromAtoB(int a, int b) {
    std::set<int> searched;
    return fromAtoBHelper(a, b, searched, KMap{mapSize, T});
}



/* Game Play */
void KGraph::play(int numSwitches, int numKeys, int numCataclysms) {
    int numTriggers = numSwitches + numKeys + numCataclysms;
    int victoryRoom = numTriggers + 1;
    KState state{mapSize, 0};
    string input;
    int room = 0, x;

    while (room != victoryRoom) {
        print(state, room);

        cout << "What room (quit)? ";
        cin >> input;

        if (input.compare("quit") == 0)
            break;

        if (!isdigit(input[0]))
            continue;

        x = stoi(input);
        if (x < size() && x != room) {
            //KMap map = fromAtoB(room, x);
            KMap map = get(room, x);
            if (map[state] == X)
                cout << "Maybe" << endl;

            if (map[state] != F) {
                room = x;
            } else {
                cout << "The door is shut. Try again." << endl;
                continue;
            }
        }

        if (room > 0 && room <= numSwitches) {
            state[x - 1] = !state[x - 1];
        } else if (room > numSwitches && room <= numTriggers) {
            state[x - 1] = true;
        }
    }

    if (room == victoryRoom) {
        print(state, room);
        cout << "Victory!" << endl;
    }
}

void KGraph::print(KState state, int room) {
    ofstream file;
    file.open("dungeon_map.dot");
    file << "graph \"dungeon\" {" << endl;

    file << "   State" << state.toCell() << " -- 0;" << endl;
    for (int i = 0; i < size(); i++) {
        if (i == room) {
            file << "   " << i << " [color=green];" << endl;
        } else {
            file << "   " << i << " [color=black];" << endl;
        }
    }
    for (int i = 0; i < size(); i++) {
        for (int j = i + 1; j < size(); j++) {
            KMap map = get(i, j);
            if (!(map.zero())) {
                list<KBox> boxes = map.solve();
                string label;
                for (list<KBox>::iterator it = boxes.begin(); it != boxes.end(); it++)
                    label += it->show() + " ";
                /*if (boxes.size() == 1) {
                    label = boxes.front().show();
                } else {
                    label = "...";
                }*/

                if (map[state] == T) {
                    file << "   " << i << " -- " << j << "[label=\"" + label + "\"];" << endl;
                } else if (map[state] == X) {
                    file << "   " << i << " -- " << j << "[label=\"" + label + "\" style=dashed];" << endl;
                } else {
                    file << "   " << i << " -- " << j << "[label=\"" + label + "\" style=dotted];" << endl;
                }
            }
        }
    }

    file << "}" << endl;
    file.close();
    system("dot -Tpdf dungeon_map.dot -o dungeon_map.pdf");
}





#ifdef DEBUG

void KGraph::test() {
    KMap::test();

    int mapSize = 4;
    KGraph g{mapSize};

    for (int i = 1; i <= mapSize; i++) {
        if (g.size() != i) {
            cerr << "size = " << g.size() << " != " << i << endl;
            exit(EXIT_FAILURE);
        }
        int x = g.add();
        if (x != i + 1) {
            cerr << "Add returned " << x << " not " << i + 1 << endl;
            exit(EXIT_FAILURE);
        }
        if (g.size() != i + 1) {
            cerr << "size = " << g.size() << " != " << i + 1 << endl;
            exit(EXIT_FAILURE);
        }
    }

    int index = 0;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (g.pairToIndex(i, j) != index) {
                cerr << "Pair -> Index [" << i << ", " << j << "] == " << g.pairToIndex(i, j) << endl;
                exit(EXIT_FAILURE);
            }
            else if (g.pairToIndex(j, i) != index) {
                cerr << "Pair -> Index [" << j << ", " << i << "] == " << g.pairToIndex(j, i) << endl;
                exit(EXIT_FAILURE);
            }
            index = g.pairToIndex(i, j) + 1;
        }
    }

    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (g.get(i, j).zero()) {
                cerr << "Construction: [" << i << ", " << j << "] != zero" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}

#endif
