#include <iostream>
#include <cstdlib>
using namespace std;

#include "kmap.hpp"
#include "kval.hpp"



KMap::KMap(int size): size{size}, cells{(vector<KVal>::size_type)(1 << size)} {
    for (int i = 0; i < cells.size(); i++)
        cells[i] = X;
}
KMap::KMap(int size, KVal const &val): size{size}, cells{(vector<KVal>::size_type)(1 << size)} {
    for (int i = 0; i < cells.size(); i++)
        cells[i] = val;
}
KMap::KMap(KState const &state): size{state.size}, cells{(vector<KVal>::size_type)(1 << (state.size))} {
    for (int i = 0; i < cells.size(); i++) {
        if (i == state.toCell()) {
            cells[i] = T;
        } else {
            cells[i] = F;
        }
    }
}
KMap::KMap(KBox const &box): size{box.size}, cells{(vector<KVal>::size_type)(1 << (box.size))} {
    for (int i = 0; i < cells.size(); i++) {
        if (box >= KState(size, i)) {
            cells[i] = T;
        } else {
            cells[i] = F;
        }
    }
}



/* Access Operators */
KVal& KMap::operator[](int index) {
    KVal *rslt;
    if (index < cells.size() && index >= 0) {
        rslt = &(cells[index]);
    } else {
        cerr << "KMap: Index " << index << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
    return *rslt;
}
KVal& KMap::operator[](KState state) {
    if (state.size != size) {
        cerr << "KMap{" << size << "} got KState{" << state.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    return operator[](state.toCell());
}



/* Constant Access Operators */
KVal KMap::operator[](int index) const {
    KVal rslt;
    if (index < cells.size() && index >= 0) {
        rslt = cells[index];
    } else {
        cerr << "KMap: Index " << index << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
    return rslt;
}
KVal KMap::operator[](KState state) const {
    if (state.size != size) {
        cerr << "KMap{" << size << "} got KState{" << state.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    return operator[](state.toCell());
}



/* Assignment Operators */
KMap& KMap::operator=(KMap const &b) {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < cells.size(); i++)
        cells[i] = b.cells[i];
    return *this;
}

KMap& KMap::operator&=(KMap const &b) {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < cells.size(); i++)
        cells[i] &= b.cells[i];
    return *this;
}

KMap& KMap::operator|=(KMap const &b) {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < cells.size(); i++)
        cells[i] |= b.cells[i];
    return *this;
}



/* Operators */
KMap KMap::operator!() const {
    KMap rslt{size};
    for (int i = 0; i < cells.size(); i++)
        rslt.cells[i] = !cells[i];
    return rslt;
}

KMap KMap::operator&(KMap const &b) const {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    KMap rslt{size};
    for (int i = 0; i < cells.size(); i++)
        rslt.cells[i] = cells[i] & b.cells[i];
    return rslt;
}

KMap KMap::operator|(KMap const &b) const {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    KMap rslt{size};
    for (int i = 0; i < cells.size(); i++)
        rslt.cells[i] = cells[i] | b.cells[i];
    return rslt;
}



/* Comparison Operators */
bool KMap::operator==(KMap const &b) const {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    bool rslt = true;
    for (int i = 0; i < cells.size(); i++)
        if (cells[i] != b[i])
            rslt = false;
    return rslt;
}
bool KMap::operator!=(KMap const &b) const {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    return !operator==(b);
}

// "A is true if B is given to be true"
bool KMap::operator>=(KMap const &b) const {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    bool rslt = true;
    for (int i = 0; i < cells.size(); i++)
        if (b[i] == T && cells[i] != T)
            rslt = false;
    return rslt;
}

// "A will always be true if B is true"
bool KMap::operator>(KMap const &b) const {
    if (b.size != size) {
        cerr << "KMap{" << size << "} got KMap{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    bool rslt = true;
    for (int i = 0; i < cells.size(); i++)
        if (b[i] != F && cells[i] != T)
            rslt = false;
    return rslt;
}

// "A is never given to be true"
bool KMap::empty() const {
    bool rslt = true;
    for (int i = 0; i < cells.size(); i++)
        if (cells[i] == T)
            rslt = false;
    return rslt;
}

// "A will never be true"
bool KMap::zero() const {
    bool rslt = true;
    for (int i = 0; i < cells.size(); i++)
        if (cells[i] != F)
            rslt = false;
    return rslt;
}





/* Computation */
list<KBox> KMap::SOP() const {
    list<KBox> rslt;
    for (int i = 0; i < cells.size(); i++)
        if (cells[i] == T)
            rslt.push_back(KState{size, i});
    return rslt;
}

KBox KMap::boundingBox() const {
    list<KBox> ts = SOP();
    if (ts.empty()) {
        cerr << "KMap: Cannot find bounding box of empty map" << endl;
        exit(EXIT_FAILURE);
    }

    list<KBox>::iterator it = ts.begin();
    KBox rslt = *it;
    for (it++; it != ts.end(); it++)
        rslt += *it;
    return rslt;
}

KBox KMap::largestBox() const {
    KMap map = maybe();
    list<KBox> boxes = map.SOP();
    if (boxes.empty()) {
        cerr << "KMap: Cannot find largest box of zero map" << endl;
        exit(EXIT_FAILURE);
    }

    for (list<KBox>::iterator it = boxes.begin(); it != boxes.end(); it++) {
        list<KBox>::iterator jt = it;
        for (jt++; jt != boxes.end() && it->termSize() == jt->termSize(); jt++) {
            if(it->difference(*jt) == 1) {
                KBox combo = *it + *jt;
                list<KBox>::iterator kt = boxes.begin();
                while (true) {
                    if (kt == boxes.end()) {
                        boxes.push_back(combo);
                        break;
                    } else if (combo == *kt) {
                        break;
                    }
                    kt++;
                }
            }
        }
    }

    if (!(((KMap)boxes.back() & !map).zero()))
        cerr << "Largest Box is not fully contained" << endl;

    return boxes.back();
}

KMap KMap::exact() const {
    KMap rslt = *this;
    for (int i = 0; i < rslt.cells.size(); i++)
        rslt.cells[i] = rslt.cells[i].exact();
    return rslt;
}

KMap KMap::maybe() const {
    KMap rslt = *this;
    for (int i = 0; i < rslt.cells.size(); i++)
        rslt.cells[i] = rslt.cells[i].maybe();
    return rslt;
}



KMap KMap::resolve() const {
    KMap rslt = *this;
    for (int i = 0; i < rslt.cells.size(); i++)
        if (rslt.cells[i] == X)
            rslt.cells[i] = F;
    return rslt;
}



list<KBox> KMap::solve() const {
    list<KBox> boxes = maybe().SOP();

    for (list<KBox>::iterator it = boxes.begin(); it != boxes.end(); it++) {
        list<KBox>::iterator jt = it;
        for (jt++; jt != boxes.end() && it->termSize() == jt->termSize(); jt++) {
            if(it->difference(*jt) == 1) {
                KBox combo = *it + *jt;
                list<KBox>::iterator kt = boxes.begin();
                while (true) {
                    if (kt == boxes.end()) {
                        boxes.push_back(combo);
                        break;
                    } else if (*kt >= combo) {
                        break;
                    }
                    kt++;
                }
            }
        }
    }

    list<KBox> ts = SOP();
    boxes.reverse();
    for (list<KBox>::iterator it = boxes.begin(); it != boxes.end();) {
        bool used = false;
        for (list<KBox>::iterator jt = ts.begin(); jt != ts.end();) {
            if (*it >= *jt) {
                used = true;
                list<KBox>::iterator kt = jt;
                jt++;
                ts.erase(kt);
            } else {
                jt++;
            }
        }

        if (!used) {
            list<KBox>::iterator jt = it;
            it++;
            boxes.erase(jt);
        } else {
            it++;
        }
    }

    return boxes;
}





#ifdef DEBUG
void assert(KVal a, KVal b, string err) {
    if (a != b) {
        cerr << err << endl;
        exit(EXIT_FAILURE);
    }
}

void KMap::test() {
    KBox::test();

    int size = 4;
    KMap m{size};

    for (int i = 0; i < size; i++) {
        if (m[i] != X) {
            cerr << "M[" << i << "] != X" << endl;
            exit(EXIT_FAILURE);
        }
        m[i] = T;
        if (m[i] != T) {
            cerr << "M[" << i << "] != T" << endl;
            exit(EXIT_FAILURE);
        }
    }


    KMap a{4}, b{4}, r{4};

    a[0] = F;
    a[1] = T;
    a[2] = F;
    a[3] = T;

    b[0] = F;
    b[1] = F;
    b[2] = T;
    b[3] = T;

    r = a;
    for (int i = 0; i < size; i++)
        assert(r[i], a[i], "r = a");

    r &= b;
    assert(r[0], F, "r &= b");
    assert(r[1], F, "r &= b");
    assert(r[2], F, "r &= b");
    assert(r[3], T, "r &= b");

    r = a;
    r |= b;
    assert(r[0], F, "r |= b");
    assert(r[1], T, "r |= b");
    assert(r[2], T, "r |= b");
    assert(r[3], T, "r |= b");

    r = !a;
    assert(r[0], T, "r = !a");
    assert(r[1], F, "r = !a");
    assert(r[2], T, "r = !a");
    assert(r[3], F, "r = !a");

    r = a & b;
    assert(r[0], F, "r = a & b");
    assert(r[1], F, "r = a & b");
    assert(r[2], F, "r = a & b");
    assert(r[3], T, "r = a & b");

    r = a | b;
    assert(r[0], F, "r = a | b");
    assert(r[1], T, "r = a | b");
    assert(r[2], T, "r = a | b");
    assert(r[3], T, "r = a | b");
}
#endif
