using T = short;
inline int get_(const T& x) { return x; }

void counting_sort(vector<T>& a) {
    int n = sz(a);
    if (n < 2) return;
    int lo = get_(a[0]), hi = lo;
    for (auto& x : a) lo = min(lo, get_(x)), hi = max(hi, get_(x));
    static vector<int> cnt; cnt.assign(hi - lo + 1, 0);
    for (auto& x : a) cnt[get_(x) - lo]++;
    for (int i = 0, s = 0; i < sz(cnt); i++) s += exchange(cnt[i], s);
    static vector<T> tmp; tmp.resize(n);
    for (auto& x : a) tmp[cnt[get_(x) - lo]++] = x;
    a.swap(tmp);
}