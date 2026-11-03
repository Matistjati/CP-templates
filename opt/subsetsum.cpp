
using u64 = uint64_t;
#include <immintrin.h>
void shift_or_avx2(vector<u64>& chunks, int w, int max_chunk) {
    const int w_words = w / 64;
    const int shift = w % 64;

    int i = max_chunk;
    if (shift == 0) {
        for (; i >= w_words + 3; i -= 4) {
            int src = i - w_words;
            __m256i curr = _mm256_loadu_si256((__m256i*)&chunks[src - 3]);
            __m256i dst = _mm256_loadu_si256((__m256i*)&chunks[i - 3]);
            _mm256_storeu_si256((__m256i*)&chunks[i - 3], _mm256_or_si256(dst, curr));
        }
    } else {
        const int comp_shift = 64 - shift;
        for (; i >= w_words + 3; i -= 4) {
            int src = i - w_words;
            u64 c0 = chunks[src - 3], c1 = chunks[src - 2], c2 = chunks[src - 1], c3 = chunks[src];
            u64 prev = (src >= 4) ? chunks[src - 4] : 0;

            __m256i curr = _mm256_setr_epi64x(c0, c1, c2, c3);
            __m256i carry = _mm256_setr_epi64x(prev, c0, c1, c2);

            __m256i hi = _mm256_slli_epi64(curr, shift);
            __m256i lo = _mm256_srli_epi64(carry, comp_shift);
            __m256i val = _mm256_or_si256(hi, lo);

            __m256i dst = _mm256_loadu_si256((__m256i*)&chunks[i - 3]);
            _mm256_storeu_si256((__m256i*)&chunks[i - 3], _mm256_or_si256(dst, val));
        }
    }

    const int comp_shift = (64 - shift) & 63;
    for (; i >= w_words; --i) {
        int high = i - w_words;
        u64 val = chunks[high] << shift;
        if (high > 0 && shift > 0) {
            val |= chunks[high - 1] >> comp_shift;
        }
        chunks[i] |= val;
    }
}

// Consider doing binary splitting before 3k -> k, 2k step
int subsetsum(span<int> weights, int t) {
    const int n_chunks = t/64+1;
    static vector<u64> chunks;
    chunks.assign(n_chunks, 0);
    chunks[0] = 1;
    int sum = 0;

    if (sz(weights)*10 >= t) { // dense: occurence map and merge
        static vector<int> occs;
        occs.assign(t+1, 0);
        for (auto w : weights) if (w<=t) occs[w]++;
        rep(w, sz(occs)) {
            int cnt = occs[w];
            if (cnt >= 3) {
                int pairs = (cnt - 1) / 2;
                if (2*w <= t) occs[2 * w] += pairs;
                cnt -= 2 * pairs;
            }
            rep(_, cnt) {
                sum += w;
                int max_chunk = min<int>(n_chunks-1, sum / 64);
                shift_or_avx2(chunks, w, max_chunk);
            }
        }
    }
    else if (sz(weights) * 100 <= t) { // ultra sparse: merge with map
        map<int,int> elements;
        for (auto w : weights) {
            elements[w]++;
        }
        u64 sum = 0;
        while (sz(elements)) {
            auto [w,cnt] = *begin(elements);
            if (w > t) break;
            elements.erase(begin(elements));
            if (cnt >= 3) {
                int pairs = (cnt - 1) / 2;
                elements[2 * w] += pairs;
                cnt -= 2 * pairs;
            }
            rep(_, cnt) {
                sum += w;
                int max_chunk = min<int>(n_chunks-1, sum / 64);
                shift_or_avx2(chunks, w, max_chunk);
            }
        }
    }
    else { // medium: do not merge (TODO: merge using sorted vector)
        repe(w, weights) {
            sum += w;
            int max_chunk = min<int>(n_chunks-1, sum / 64);
            shift_or_avx2(chunks, w, max_chunk);
        }
    }

    // optionally, return chunks
    return (chunks[t/64] >> (t%64)) & 1;
}
