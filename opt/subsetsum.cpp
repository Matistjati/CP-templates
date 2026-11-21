#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

using u64 = uint64_t;
#include <immintrin.h>
static inline __m256i ld(const u64* p) { return _mm256_loadu_si256((const __m256i*)p); }
static inline void st(u64* p, __m256i v) { _mm256_storeu_si256((__m256i*)p, v); 

void shift_or_avx2(u64* __restrict chunks, int w, int max_chunk) {
    const int ww = w >> 6, sh = w & 63;
    int i = max_chunk;
    if (sh == 0) {
        for (; i >= ww + 7; i -= 8) {
            u64* d = chunks + i - 7; const u64* s = d - ww;
            __m256i a = _mm256_or_si256(ld(d + 4), ld(s + 4));
            __m256i b = _mm256_or_si256(ld(d), ld(s));
            st(d + 4, a); st(d, b);
        }
    } else {
        const __m128i cs = _mm_cvtsi32_si128(sh), cc = _mm_cvtsi32_si128(64 - sh);
        for (; i >= ww + 8; i -= 8) {
            u64* d = chunks + i - 7; const u64* s = d - ww;
            __m256i a = _mm256_or_si256(_mm256_sll_epi64(ld(s + 4), cs), _mm256_srl_epi64(ld(s + 3), cc));
            __m256i b = _mm256_or_si256(_mm256_sll_epi64(ld(s), cs), _mm256_srl_epi64(ld(s - 1), cc));
            a = _mm256_or_si256(a, ld(d + 4));
            b = _mm256_or_si256(b, ld(d));
            st(d + 4, a); st(d, b);
        }
    }
    for (; i >= ww; --i) {
        u64 v = chunks[i - ww] << sh;
        if (sh && i > ww) v |= chunks[i - ww - 1] >> (64 - sh);
        chunks[i] |= v;
    }
}

template<int B = 8, int P = 4>
void radix_sort(vector<int>& a) {
    int n = a.size();
    if (n < 256) { sort(a.begin(), a.end()); return; }
    constexpr int K = 1 << B, M = K - 1;
    static int cnt[P][K];
    memset(cnt, 0, sizeof cnt);

    for (int x : a)
        for (int p = 0; p < P; p++) cnt[p][(x >> (p * B)) & M]++;

    static vector<int> buf;
    if ((int)buf.size() < n) buf.resize(n);
    int* src = a.data();
    int* dst = buf.data();

    for (int p = 0; p < P; p++) {
        int* c = cnt[p];
        if (c[(src[0] >> (p * B)) & M] == n) continue;
        for (int i = 0, s = 0; i < K; i++) s += exchange(c[i], s);
        for (int i = 0; i < n; i++) dst[c[(src[i] >> (p * B)) & M]++] = src[i];
        swap(src, dst);
    }
    if (src != a.data()) memcpy(a.data(), src, n * sizeof(int));
}

// Bitset of all subset sums in [0, t]. Weights must be >= 0.
vector<u64> subsetsum(vector<int> weights, int t) {
    assert(t >= 0);
    const int n_chunks = t/64+1;
    static vector<u64> chunks;
    chunks.assign(n_chunks, 0);
    chunks[0] = 1;

    for (int w : weights) assert(w >= 0);
    erase_if(weights, [&](int w) { return w == 0 || w > t; });

    u64 sum = 0;
    bool switched = false;
    auto fill_prefix = [&](u64 upto) {
        u64 lim = min<u64>(upto, t) + 1, i = 0;
        for (; i + 64 <= lim; i += 64) chunks[i/64] = ~0ULL;
        if (u64 r = lim - i) chunks[i/64] |= (1ULL << r) - 1;
    };
    auto process = [&](int x) {
        if (!switched) {
            if (x > sum+1) {
                fill_prefix(min<u64>(t, sum));
                switched = true;
            }
        }
        sum += x;
        if (switched) {
            int max_chunk = (int)min<u64>(n_chunks-1, sum / 64);
            shift_or_avx2(&chunks[0], x, max_chunk);
        }
    };
    if (sz(weights)*10 >= t) { // dense: occurence map and merge
        static vector<int> occs;
        occs.assign((size_t)t+1, 0);
        for (auto w : weights) occs[w]++;
        rep(w, sz(occs)) {
            int cnt = occs[w];
            if (cnt >= 3) {
                int pairs = (cnt - 1) / 2;
                if (2*w <= t) occs[2 * w] += pairs;  // w is ll here
                cnt -= 2 * pairs;
            }
            rep(_, cnt) {
                process(w);
            }
        }
    }
    else { // sparse-ish: binary split
        vector<int> split_w;
        radix_sort(weights);
        split_w.reserve(sz(weights)*5);
        int i = 0;
        while (i < sz(weights)) {
            int j = i;
            while (j < sz(weights) && weights[i] == weights[j]) j++;
            if (weights[i] <= t) {
                ll c = j-i, x = weights[i];
                for (ll p = 1; c >= p; p *= 2) {
                    if (x*p <= t) split_w.push_back(x*p);
                    c -= p;
                }
                if (c && x*c <= t) split_w.push_back(x*c);
            }
            i=j;
        }
        radix_sort(split_w);
        for (auto x : split_w) process(x);
    }
    if (!switched) {
        fill_prefix(min<u64>(t, sum));
    }

    if (int r = (t + 1) % 64) chunks.back() &= (1ULL << r) - 1;

    // optionally, return chunks
    return chunks;
}
