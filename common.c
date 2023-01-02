#include "common.h"

ull isqrt(ull i)
{
    ull l = 0;
    ull r = i + 1;
    while (l != r - 1)
    {
        ull m = (l + r) / 2;
        if (m * m <= i)
            l = m;
        else
            r = m;
    }
    return l;
}

