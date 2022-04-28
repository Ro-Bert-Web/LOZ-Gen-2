#ifndef KVAL_HPP
#define KVAL_HPP

enum KVal_Val {
    X = 0,
    T,
    F
};

class KVal {
    KVal_Val val;
public:
    KVal();
    KVal(KVal_Val);

    KVal& operator=(KVal const&);
    KVal& operator&=(KVal const&);
    KVal& operator|=(KVal const&);

    KVal operator!() const;
    KVal operator&(KVal const&) const;
    KVal operator|(KVal const&) const;

    bool operator==(KVal const&) const;
    bool operator!=(KVal const&) const;
    bool operator>=(KVal const&) const;
    bool operator>(KVal const&) const;

    KVal exact() const;
    KVal maybe() const;
};

#endif
