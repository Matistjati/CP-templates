# Dont forget bmi2
int permToInt(vector<int>& v) {
    int use = 0, i = 0, r = 0;
    for(int x:v) r = r * ++i + __builtin_popcount(use & -(1<<x)),
        use |= 1 << x;                     // (note: minus, not ~!)
    return r;
}

#include <immintrin.h>
vector<int> intToPerm(int r, int n) {
    vector<int> v(n);
    if (n <= 0) return v;

    unsigned int avail = (1U << n) - 1;
    unsigned int ur = r;
    int* out = v.data();

#define STEP(i) \
    { \
        unsigned int c = ur % i; ur /= i; \
        unsigned int mask = _pdep_u32(1U << (i - 1 - c), avail); \
        out[i - 1] = __builtin_ctz(mask); \
        avail ^= mask; \
    }

#define STEP1 { out[0] = __builtin_ctz(avail); }

    switch (n) {
        case 11: STEP(11) STEP(10) STEP(9) STEP(8) STEP(7) STEP(6) STEP(5) STEP(4) STEP(3) STEP(2) STEP1 break;
        case 10: STEP(10) STEP(9) STEP(8) STEP(7) STEP(6) STEP(5) STEP(4) STEP(3) STEP(2) STEP1 break;
        case 9:  STEP(9)  STEP(8) STEP(7) STEP(6) STEP(5) STEP(4) STEP(3) STEP(2) STEP1 break;
        case 8:  STEP(8)  STEP(7) STEP(6) STEP(5) STEP(4) STEP(3) STEP(2) STEP1 break;
        case 7:  STEP(7)  STEP(6) STEP(5) STEP(4) STEP(3) STEP(2) STEP1 break;
        case 6:  STEP(6)  STEP(5) STEP(4) STEP(3) STEP(2) STEP1 break;
        case 5:  STEP(5)  STEP(4) STEP(3) STEP(2) STEP1 break;
        case 4:  STEP(4)  STEP(3) STEP(2) STEP1 break;
        case 3:  STEP(3)  STEP(2) STEP1 break;
        case 2:  STEP(2)  STEP1 break;
        case 1:  STEP1 break;
        default: __builtin_unreachable();
    }

#undef STEP
#undef STEP1

    return v;
}

