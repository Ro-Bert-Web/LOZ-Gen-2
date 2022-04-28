#include "kval.hpp"

KVal::KVal() {
    val = X;
}
KVal::KVal(KVal_Val val) {
    this->val = val;
}



/* Assignment Operators */
KVal& KVal::operator=(KVal const &b) {
    val = b.val;
    return *this;
}

KVal& KVal::operator&=(KVal const &b) {
    if (val == T && b == T) {
        val = T;
    } else if (val == F || b == F) {
        val = F;
    } else {
        val = X;
    }
    return *this;
}

KVal& KVal::operator|=(KVal const &b) {
    if (val == T || b == T) {
        val = T;
    } else if (val == F && b == F) {
        val = F;
    } else {
        val = X;
    }
    return *this;
}



/* Operators */
KVal KVal::operator!() const {
    KVal rslt;
    if (val == T) {
        rslt = F;
    } else if (val == F) {
        rslt = T;
    } else {
        rslt = X;
    }
    return rslt;
}

KVal KVal::operator&(KVal const &b) const {
    KVal rslt;
    if (val == T && b == T) {
        rslt = T;
    } else if (val == F || b == F) {
        rslt = F;
    } else {
        rslt = X;
    }
    return rslt;
}

KVal KVal::operator|(KVal const &b) const {
    KVal rslt;
    if (val == T || b == T) {
        rslt = T;
    } else if (val == F && b == F) {
        rslt = F;
    } else {
        rslt = X;
    }
    return rslt;
}



/* Comparison Operators */
bool KVal::operator==(KVal const &b) const {
    return val == b.val;
}
bool KVal::operator!=(KVal const &b) const {
    return val != b.val;
}

// A is true if B is given to be true
bool KVal::operator>=(KVal const &b) const {
    return !(b == T && val != T);
}

// A is always true if B is true
bool KVal::operator>(KVal const &b) const {
    return !(b != F && val != T);
}



KVal KVal::exact() const {
    KVal rslt = val;
    if (val == X)
        rslt = F;
    return rslt;
}

KVal KVal::maybe() const {
    KVal rslt = val;
    if (val == X)
        rslt = T;
    return rslt;
}
