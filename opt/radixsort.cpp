
using ull = unsigned long long;
using T = ull;
uint64_t key_(const T& x) { return x; } // Does not handle negative numbers

//assert(2^(P*B) > max(a)). B=8 is usually a good choise
template<int B, int P>
void radix_sort(vector<T>& a) {
    const int M = (1 << B) - 1;
    int n = sz(a);
    if (n < 2) return;
    static int cnt[P][1 << B];
    memset(cnt, 0, sizeof cnt);
    for (auto& x : a) {
        uint64_t k = key_(x);
        rep(p,P) cnt[p][k >> p*B & M]++;
    }
    static vector<T> tmp;
    tmp.resize(n);
    rep(p,P) {
        int* c = cnt[p];
        if (c[key_(a[0]) >> p*B & M] == n) continue;
        for (int i = 0, s = 0; i <= M; i++) s += exchange(c[i], s);
        for (auto& x : a) tmp[c[key_(x) >> p*B & M]++] = x;
        a.swap(tmp);
    }
}
