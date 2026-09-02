#pragma GCC target("avx2")
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <immintrin.h>
struct FioShuf {
    unsigned char t[2][17][16];
    constexpr FioShuf() : t{} {
        for (int neg = 0; neg < 2; neg++)
            for (int len = 0; len <= 16; len++) {
                int off = 16 - (len - neg);
                for (int k = 0; k < 16; k++)
                    t[neg][len][k] = len >= neg && k >= off
                                   ? (unsigned char)(k - off + neg) : 0x80;
            }
    }
};
static constexpr FioShuf SH{};
struct FastInput {
    const char *base, *p, *end;
    uint64_t mask;
    bool ok = true;
    static uint64_t wmask(const char *q) {   // one bit per byte <= ' '
        __m256i sp = _mm256_set1_epi8(' ');  // unsigned cmp, so UTF-8 is a token
        uint32_t m0 = _mm256_movemask_epi8(_mm256_cmpeq_epi8(sp,
            _mm256_max_epu8(sp, _mm256_loadu_si256((const __m256i*)q))));
        uint32_t m1 = _mm256_movemask_epi8(_mm256_cmpeq_epi8(sp,
            _mm256_max_epu8(sp, _mm256_loadu_si256((const __m256i*)(q + 32)))));
        return m0 | ((uint64_t)m1 << 32);
    }
    FastInput() {
        struct stat st;
        fstat(0, &st);
        size_t sz = st.st_size, r = (sz + 4095) & ~4095UL;
        char *q = (char*)mmap(nullptr, r + 4096, PROT_READ,
                              MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        mmap(q, sz, PROT_READ, MAP_PRIVATE | MAP_FIXED | MAP_POPULATE, 0, 0);
        base = p = q;
        end = q + sz;
        mask = wmask(q);
    }
    static uint64_t pack16(__m128i x) {
        x = _mm_and_si128(x, _mm_set1_epi8(0x0F));
        x = _mm_maddubs_epi16(x, _mm_set1_epi16(0x010A));    // 10, 1
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00010064));   // 100, 1
        x = _mm_packus_epi32(x, x);
        x = _mm_madd_epi16(x, _mm_set1_epi32(0x00012710));   // 10000, 1
        return (uint64_t)(uint32_t)_mm_cvtsi128_si32(x) * 100000000ULL
             + (uint32_t)_mm_extract_epi32(x, 1);
    }
    const char* token(long& len) {
        for (;;) {
            if (p >= end) { ok = false; len = 0; return p; }
            while (!mask) { base += 64; mask = wmask(base); }
            const char *d = base + __builtin_ctzll(mask), *s = p;
            mask &= mask - 1;
            p = d + 1;
            if (d != s) { len = d - s; return s; }
        }
    }
    template<typename T> static T num(const char *s, long len) {
        bool neg = false;
        if constexpr (std::is_signed_v<T>) neg = *s == '-';
        std::make_unsigned_t<T> u;
        if (__builtin_expect(len > 16, 0)) {   // wider than one register: peel the
            long k = len - neg - 16;           // top digits, then the last 16 are
            uint64_t pre = 0;                  // already aligned, no shuffle
            for (long i = 0; i < k; i++) pre = pre * 10 + (unsigned char)s[neg + i] - '0';
            u = pre * 10000000000000000ULL
              + pack16(_mm_loadu_si128((const __m128i*)(s + len - 16)));
        } else {
            u = pack16(_mm_shuffle_epi8(_mm_loadu_si128((const __m128i*)s),
                       _mm_loadu_si128((const __m128i*)SH.t[neg][len])));
        }
        return neg ? T(0) - T(u) : T(u);
    }
    // False once a read ran off the end, so `while (fio >> x)` reads to EOF.
    // Without it that loop walks off the mapping.
    explicit operator bool() const { return ok; }
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    FastInput& operator>> (T& x) {
        long len;
        const char *s = token(len);
        x = num<T>(s, len);
        return *this;
    }
    template<typename T> void read(T *a, long n) {
        const char *b = base, *q = p;
        uint64_t m = mask;
        for (; n; n--) {
            const char *s = q, *d;
            for (;;) {                                  // token(), minus the
                while (!m) { b += 64; m = wmask(b); }   // EOF test
                d = b + __builtin_ctzll(m);
                m &= m - 1;
                if (d != s) break;
                s = d + 1;
            }
            *a++ = num<T>(s, d - s);
            q = d + 1;
        }
        base = b; p = q; mask = m;   // leaves p where token() would, so >> and
    }                                // read() interleave freely
    // Fills v.size() elements, or n of them, growing v if it is short: both
    // `vector<int> a(n); cin.read(a);` and `a.reserve(n); cin.read(a, n);` work.
    template<typename T> void read(std::vector<T>& v, long n = -1) {
        if (n < 0) n = v.size();
        if ((size_t)n > v.size()) v.resize(n);
        read(v.data(), n);
    }
    // `cin >> v` is read(v): fills v.size() elements, so size v first.
    template<typename T> FastInput& operator>> (std::vector<T>& v) {
        read(v);
        return *this;
    }
    FastInput& operator>> (std::string& s) {
        long len;
        const char *t = token(len);
        s.assign(t, len);
        return *this;
    }
    // Zero-copy: the token stays in the mapping, so the view is good for the
    // rest of the run.  Fastest way to read strings.
    FastInput& operator>> (std::string_view& s) {
        long len;
        const char *t = token(len);
        s = std::string_view(t, len);
        return *this;
    }
    FastInput& operator>> (char *s) {   // caller needs room for len + 1 bytes
        long len;
        const char *t = token(len);
        memcpy(s, t, len);
        s[len] = 0;
        return *this;
    }
    // Consumes a whole token and keeps its first byte -- fine for tokens that
    // are one char, but read a space-less grid row as a string, not char by
    // char.
    FastInput& operator>> (char& c) {
        long len;
        c = *token(len);
        return *this;
    }
    FastInput& operator>> (double& x) {   // rare enough not to be worth vectorizing
        long len;
        const char *t = token(len);
        std::from_chars(t, t + len, x);
        return *this;
    }
} scanner;
#define cin scanner
