#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

alignas(64) static char kD2[200], kD4[10000][4];
static struct BuildTables {
    BuildTables() {
        char* q = kD2;
        for (int a = 0; a < 10; a++)
            for (int b = 0; b < 10; b++) { *q++ = char('0' + a); *q++ = char('0' + b); }
        q = kD4[0];
        for (int a = 0; a < 100; a++)
            for (int b = 0; b < 100; b++) {
                *q++ = kD2[2 * a]; *q++ = kD2[2 * a + 1];
                *q++ = kD2[2 * b]; *q++ = kD2[2 * b + 1];
            }
    }
} kBuildTables;

static inline uint32_t d4(uint32_t i) { uint32_t v; memcpy(&v, kD4[i], 4); return v; }
static inline uint64_t blk8(uint32_t x) {
    return (uint64_t)d4(x / 10000) | (uint64_t)d4(x % 10000) << 32;
}

static inline char* emit8(char* p, uint32_t x) {
    uint64_t v = blk8(x);
    unsigned sh = (unsigned)__builtin_ctzll(
        (v ^ 0x3030303030303030ULL) | 1ULL << 56) & 56;
    uint64_t w = v >> sh;
    memcpy(p, &w, 8);
    return p + 8 - (sh >> 3);
}
static inline char* emit32(char* p, uint32_t x) {
    if (x < 100000000u) return emit8(p, x);
    uint32_t hi = x / 100000000u;
    uint64_t lo = blk8(x - hi * 100000000u);
    if (hi >= 10) { memcpy(p, kD2 + hi * 2, 2); memcpy(p + 2, &lo, 8); return p + 10; }
    *p = char('0' + hi);
    memcpy(p + 1, &lo, 8);
    return p + 9;
}
static inline char* emit64(char* p, uint64_t x) {
    if (x < 100000000ull) return emit8(p, (uint32_t)x);
    if (x < 10000000000000000ull) {
        uint64_t lo = blk8((uint32_t)(x % 100000000ull));
        p = emit8(p, (uint32_t)(x / 100000000ull));      // strips leading zeros
        memcpy(p, &lo, 8);
        return p + 8;
    }
    uint64_t rest = x % 10000000000000000ull;            // 17..20 digits
    uint64_t mid = blk8((uint32_t)(rest / 100000000ull));
    uint64_t lo  = blk8((uint32_t)(rest % 100000000ull));
    p = emit8(p, (uint32_t)(x / 10000000000000000ull));  // 1..1844
    memcpy(p, &mid, 8);
    memcpy(p + 8, &lo, 8);
    return p + 16;
}
// Sign handling: for unsigned T this whole block compiles away.
template<class T>
static inline char* emitval(char* p, T x) {
    using U = std::make_unsigned_t<T>;
    U u = (U)x;
    if constexpr (std::is_signed_v<T>) {
        U m = (U)(x >> (sizeof(T) * 8 - 1));   // all ones iff x < 0
        u = (U)((u ^ m) - m);                  // |x|, branchlessly
        *p = '-';                              // stored always, overwritten when
        p += m & 1;                            // ...x >= 0, by not advancing
    }
    if constexpr (sizeof(U) <= 4) return emit32(p, (uint32_t)u);
    else return emit64(p, (uint64_t)u);
}

struct FastOutput {
    // 32 KB: from 32 KB up, a file's write() cost has flattened out (3.0 of a
    // possible 3.2 GB/s here), while staying inside a pipe's 64 KB capacity so a
    // custom output validator on the far end never makes us block mid-write --
    // 256 KB writes measured 3x worse than 32 KB ones into a pipe.
    static const int BUF = 1 << 15;
    char b[BUF + 64], *p = b;                 // slack: stores round up to 8 or 16

    static void raw(const char* q, size_t n) {
        while (n) {                           // a write to a pipe may be partial
            ssize_t k = ::write(1, q, n);
            if (k <= 0) return;
            q += k; n -= (size_t)k;
        }
    }
    void flush() { raw(b, (size_t)(p - b)); p = b; }   // safe to call twice
    ~FastOutput() { flush(); }
    // Call before writing n bytes; n must not exceed BUF.
    inline void need(size_t n) { if (p + n > b + BUF) flush(); }

    template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
    FastOutput& operator<< (T x) {
        need(24);                             // sign + 20 digits, rounded up
        p = emitval(p, x);
        return *this;
    }
    FastOutput& operator<< (bool v) { need(1); *p++ = char('0' + v); return *this; }
    FastOutput& operator<< (char c) { need(1); *p++ = c; return *this; }

    FastOutput& put(const char* s, size_t n) {
        if (n > (size_t)BUF) { flush(); raw(s, n); return *this; }   // too big to buffer
        need(n);
        memcpy(p, s, n);
        p += n;
        return *this;
    }
    FastOutput& operator<< (const char* s) { return put(s, strlen(s)); }
    FastOutput& operator<< (std::string_view s) { return put(s.data(), s.size()); }

    // Space-separated, newline at the end; pass sep/end to change that, end = 0
    // for no terminator.  Both the room check and the buffer pointer are hoisted
    // out of the inner loop: we work out how many items certainly fit, then run
    // that many with p in a register.
    template<class T>
    void print(const std::vector<T>& v, char sep = ' ', char end = '\n') {
        size_t i = 0, n = v.size();
        while (i < n) {
            size_t fits = (size_t)(b + BUF - p) / 24;
            if (!fits) { flush(); continue; }
            size_t j = i + (fits < n - i ? fits : n - i);
            char* q = p;
            for (; i < j; i++) { q = emitval(q, v[i]); *q++ = sep; }
            p = q;
        }
        if (n) { if (end) p[-1] = end; else p--; }
        else if (end) { need(1); *p++ = end; }
    }
} printer;
#define cout printer
