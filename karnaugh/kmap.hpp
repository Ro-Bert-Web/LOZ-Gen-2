#ifndef KMAP_HPP
#define KMAP_HPP

#include <vector>
#include <list>
#include <string>
using namespace std;

#include "kval.hpp"
#include "kstate.hpp"
#include "kbox.hpp"

class KMap {
    vector<KVal> cells;
public:
    const int size;

    KMap(int);
    KMap(int, KVal const&);
    KMap(KState const&);
    KMap(KBox const&);

    KVal& operator[](int);
    KVal& operator[](KState);

    KVal operator[](int) const;
    KVal operator[](KState) const;

    KMap& operator=(KMap const&);
    KMap& operator&=(KMap const&);
    KMap& operator|=(KMap const&);

    KMap operator!() const;
    KMap operator&(KMap const&) const;
    KMap operator|(KMap const&) const;

    bool operator==(KMap const&) const;
    bool operator!=(KMap const&) const;
    bool operator>=(KMap const&) const;
    bool operator>(KMap const&) const;
    bool empty() const;
    bool zero() const;

    list<KBox> SOP() const;
    KBox boundingBox() const;
    KBox largestBox() const;
    KMap exact() const;
    KMap maybe() const;
    KMap resolve() const;
    list<KBox> solve() const;

#ifdef DEBUG
    static void test();
#endif
};

#endif
