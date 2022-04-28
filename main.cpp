#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include "spuzzle/spuzzle.hpp"

using namespace std;

int main(int argc, char **argv) {
    int numSwitches = 4;
    int numKeys = 0;
    int numCataclysms = 0;
    int pressure = 100;
    int simplicity = 2;

    if (argc > 1) {
        numSwitches = atoi(argv[1]);
        if (argc > 2) {
            numKeys = atoi(argv[2]);
        } else {
            numKeys = 0;
        }
        if (argc > 3) {
            numCataclysms = atoi(argv[3]);
        } else {
            numCataclysms = 0;
        }
        if (argc > 4) {
            pressure = atoi(argv[4]);
        } else {
            pressure = 100;
        }
        if (argc > 5) {
            simplicity = atoi(argv[5]);
        } else {
            simplicity = 0;
        }
    }

    KGraph graph = spuzzle(numSwitches, numKeys, numCataclysms, pressure, simplicity);
    system("dot -Tpdf state_diagram.dot -o state_diagram.pdf");
    graph.play(numSwitches, numKeys, numCataclysms);

    return 1;
}
