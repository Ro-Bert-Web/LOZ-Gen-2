#include <iostream>
#include <cstdlib>
using namespace std;

#include "kstate.hpp"

KState::KState(int size): size{size} {
    state.resize(size);
    for (int i = 0; i < state.size(); i++)
        state[i] = false;
}
KState::KState(int size, int cell): size{size} {
    state.resize(size);
    for (int i = 0; i < state.size(); i++) {
        if (cell & (1 << i)) {
            state[i] = true;
        } else {
            state[i] = false;
        }
    }
}



/* Translate State to Cell */
int KState::toCell() const {
    int rslt = 0;
    for (int i = 0; i < state.size(); i++)
        if (state[i])
            rslt |= 1 << i;
    return rslt;
}



/* Access Operator */
bool& KState::operator[](int index) {
    bool *rslt;
    if (index < state.size() && index >= 0) {
        rslt = (bool*)&(state[index]);
    } else {
        cerr << "KState: Index " << index << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
    return *rslt;
}



/* Constant Access Operator */
bool KState::operator[](int index) const {
    bool rslt;
    if (index < state.size() && index >= 0) {
        rslt = state[index];
    } else {
        cerr << "KState: Index " << index << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
    return rslt;
}



/* Assignement Operators */
KState& KState::operator=(KState const &b) {
    for (int i = 0; i < state.size(); i++)
        state[i] = (char)b[i];
    return *this;
}
