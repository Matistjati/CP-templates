template<class T>
struct CSR {
    vector<int> st;
    vector<T> adj;
    vector<pair<int, T>> pending;
    // n: number of rows. m: expected number of items
    CSR(int n = 0, int m = 0) : st(n + 1) { pending.reserve(m); }
    void add(int u, const T& x) { pending.emplace_back(u, x); }
    void build() {
        for (auto& [u, x] : pending) st[u+1]++;
        partial_sum(all(st), begin(st));
        adj.resize(sz(pending));
        vector<int> pos(begin(st), end(st) - 1);
        for (auto& [u, x] : pending) adj[pos[u]++] = x;
        vector<pair<int, T>>().swap(pending);
    }
    span<T> operator[](int u) { return {adj.data() + st[u], adj.data() + st[u+1]}; }
};
