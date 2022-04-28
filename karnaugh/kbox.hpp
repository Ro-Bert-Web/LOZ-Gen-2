#ifndef KBOX_HPP
#define KBOX_HPP

#include <vector>
using namespace std;

#include "kval.hpp"
#include "kstate.hpp"

class KBox {
    vector<KVal> term;
public:
    const int size;

    KBox(int);
    KBox(KState const&);

    KVal& operator[](int);
    KVal operator[](int) const;

    KBox& operator=(KBox const&);
    KBox& operator+=(KBox const&);

    KBox operator+(KBox const&) const;

    bool operator==(KBox const&) const;
    bool operator!=(KBox const&) const;
    // Whether A fully contains B
    bool operator>=(KBox const&) const;
    // How much B is outside A
    int difference(KBox const&) const;

    int termSize();

    string show();

    #ifdef DEBUG
    static void test();
    #endif
};

#endif
