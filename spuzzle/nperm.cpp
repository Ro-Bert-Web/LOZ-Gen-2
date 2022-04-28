#include "nperm.hpp"

int fact(int x) {
    int rslt;
    if (x > 1) {
        rslt = x * fact(x - 1);
    } else {
        rslt = 1;
    }
    return rslt;
}

list<int> nperm(int code, int n) {
    list<int> rslt;

    if (n > 0) {
        int factx = fact(n - 1);
        int i = code / factx;

        rslt = nperm(code % factx, n - 1);
        for (list<int>::iterator it = rslt.begin(); it != rslt.end(); it++)
            if (*it >= i)
                (*it)++;
        rslt.push_front(i);
    }

    return rslt;
}
