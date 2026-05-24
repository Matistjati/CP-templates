
using T = tuple<int, int, int>;
int get_(const T& x) {return get<0>(x);}
void radix_sort_pairs(vector<T>& a) {
    const int B = 8;
    const int MASK = (1 << B) - 1;
    const int PASSES = (32 + B - 1) / B;

    vector<T> tmp(a.size());
    int pref[1 << B];
    for (int pass = 0; pass < PASSES; pass++) {
        int shift = pass * B;

        int cnt[1 << B] = { 0 };
        for (auto& x : a) cnt[(get_(x) >> shift) & MASK]++;

        pref[0] = 0;
        for (int i = 1; i < (1 << B); i++) pref[i] = pref[i - 1] + cnt[i - 1];

        for (auto& x : a) {
            int d = (get_(x) >> shift) & MASK;
            tmp[pref[d]++] = x;
        }
        a.swap(tmp);
    }
}

