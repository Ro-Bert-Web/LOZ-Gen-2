#ifndef KSTATE_HPP
#define KSTATE_HPP

#include <vector>
using namespace std;

class KState {
    vector<char> state;
public:
    const int size;

    KState(int);
    KState(int, int);

    int toCell() const;

    bool& operator[](int);
    bool operator[](int) const;

    KState& operator=(KState const&);
};

#endif
