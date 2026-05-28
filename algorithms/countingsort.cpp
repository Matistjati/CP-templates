using T = short;
inline int get_(const T& x) { return x; }

void counting_sort(vector<T>& a) {
    int n = a.size();

    int K = get_(a[0]);
    rep(i,n) K = max(K, get_(a[i]));

    vector<int> cnt(K + 2);
    rep(i,n) cnt[get_(a[i])+1]++;
    partial_sum(all(cnt),begin(cnt));

    vector<T> tmp(n);
    for (int i = 0; i < n; i++) {
        tmp[cnt[get_(a[i])]++] = a[i];
    }
    a.swap(tmp);
}
