
typedef tuple<int, int, int> T;
void radix_sort_pairs(vector<T>& a) {
    const int B = 8;
    const int MASK = (1 << B) - 1;
    const int PASSES = (32 + B - 1) / B;

    vector<T> tmp(a.size());
    for (int pass = 0; pass < PASSES; pass++) {
        int shift = pass * B;

        int cnt[1 << B] = { 0 };
        for (auto& x : a)
            cnt[(get<0>(x) >> shift) & MASK]++; // change this

        int pref[1 << B];
        pref[0] = 0;
        for (int i = 1; i < (1 << B); i++)
            pref[i] = pref[i - 1] + cnt[i - 1];

        for (auto& x : a) {
            int d = (get<0>(x) >> shift) & MASK; // and this
            tmp[pref[d]++] = x;
        }
        a.swap(tmp);
    }
}

