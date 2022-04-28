#include <iostream>
#include <cstdlib>
using namespace std;

#include "kbox.hpp"



KBox::KBox(int size): size{size}, term{(vector<KVal>::size_type)size} {
    for (int i = 0; i < term.size(); i++)
        term[i] = X;
}
KBox::KBox(KState const &state): size{state.size}, term{(vector<KVal>::size_type)(state.size)} {
    for (int i = 0; i < term.size(); i++) {
        if (state[i]) {
            term[i] = T;
        } else {
            term[i] = F;
        }
    }
}



/* Access Operator */
KVal& KBox::operator[](int index) {
    KVal *rslt;
    if (index < term.size() && index >= 0) {
        rslt = &(term[index]);
    } else {
        cerr << "KBox: Index " << index << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
    return *rslt;
}



/* Constant Access Operator */
KVal KBox::operator[](int index) const {
    KVal rslt;
    if (index < term.size() && index >= 0) {
        rslt = term[index];
    } else {
        cerr << "KBox: Index " << index << " out of bounds" << endl;
        exit(EXIT_FAILURE);
    }
    return rslt;
}



/* Assignment Operators */
KBox& KBox::operator=(KBox const &b) {
    if (b.size != size) {
        cerr << "KBox{" << size << "} got KBox{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < term.size(); i++)
        term[i] = b[i];
    return *this;
}

KBox& KBox::operator+=(KBox const &b) {
    if (b.size != size) {
        cerr << "KBox{" << size << "} got KBox{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < term.size(); i++)
        if (term[i] != b[i])
            term[i] = X;
    return *this;
}



/* Operators */
KBox KBox::operator+(KBox const &b) const {
    if (b.size != size) {
        cerr << "KBox{" << size << "} got KBox{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    KBox rslt{size};
    for (int i = 0; i < term.size(); i++) {
        if (term[i] != b[i]) {
            rslt[i] = X;
        } else {
            rslt[i] = term[i];
        }
    }
    return rslt;
}



/* Comparison Operators */
bool KBox::operator==(KBox const &b) const {
    if (b.size != size) {
        cerr << "KBox{" << size << "} got KBox{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    bool rslt = true;
    for (int i = 0; i < term.size(); i++)
        if (term[i] != b[i])
            rslt = false;
    return rslt;
}

bool KBox::operator!=(KBox const &b) const {
    return !operator==(b);
}

bool KBox::operator>=(KBox const &b) const {
    if (b.size != size) {
        cerr << "KBox{" << size << "} got KBox{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    bool rslt = true;
    for (int i = 0; i < term.size(); i++)
        if (term[i] != X && term[i] != b[i])
            rslt = false;
    return rslt;
}

// How much B is outside the box of A
int KBox::difference(KBox const &b) const {
    if (b.size != size) {
        cerr << "KBox{" << size << "} got KBox{" << b.size << "}" << endl;
        exit(EXIT_FAILURE);
    }
    int rslt = 0;
    for (int i = 0; i < term.size(); i++)
        if (term[i] != b[i])
            rslt++;
    return rslt;
}



/* Getters */
int KBox::termSize() {
    int rslt = 0;
    for (int i = 0; i < term.size(); i++)
        if (term[i] != X)
            rslt++;
    return rslt;
}



/* Print */
string KBox::show() {
    string rslt;
    for (int i = 0; i < term.size(); i++) {
        if (term[i] == T) {
            rslt += (char)('a' + i);
        } else if (term[i] == F) {
            rslt += "~";
            rslt += (char)('a' + i);
        }
    }
    return rslt;
}





#ifdef DEBUG
void KBox::test() {
    int size = 4;
    KBox b{size}, a{size};

    for (int i = 0; i < size; i++) {
        if (b[i] != X) {
            cerr << "B[" << i << "] != X" << endl;
            exit(EXIT_FAILURE);
        }
        b[i] = T;
        if (b[i] != T) {
            cerr << "B[" << i << "] != T" << endl;
            exit(EXIT_FAILURE);
        }
    }

    b = KState{size, 1};
    a = KState{size, 2};
    for (int i = 0; i < size; i++) {
        if (i == 1) {
            if (a[i] != T) {
                cerr << "A[" << i << "] != T" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else {
            if (a[i] != F) {
                cerr << "A[" << i << "] != F" << endl;
                exit(EXIT_FAILURE);
            }
        }

        if (i == 0) {
            if (b[i] != T) {
                cerr << "B[" << i << "] != T" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else {
            if (b[i] != F) {
                cerr << "B[" << i << "] != F" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    b += a;
    for (int i = 0; i < size; i++) {
        if (i == 0 || i == 1) {
            if (b[i] != X) {
                cerr << "B[" << i << "] != X" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else {
            if (b[i] != F) {
                cerr << "B[" << i << "] != F" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}
#endif
