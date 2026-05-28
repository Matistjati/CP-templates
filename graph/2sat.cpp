struct SatSolver {
    int n, timer = 0, scc = 0;
    vector<std::vector<int>> g;
    vector<int> low, id, comp, stk, ans;

    SatSolver(int n) : n(n), g(2*n), low(2*n), id(2*n), comp(2*n, -1), ans(n) {}

    int T(int i) const { return i * 2; }
    int F(int i) const { return i * 2 + 1; }
    int get(int i, int flip) {return flip ? T(i) : F(i); }

    void implies(int u, int v) {
        g[u].push_back(v);
        g[v ^ 1].push_back(u ^ 1);
    }

    void dfs(int u) {
        low[u] = id[u] = ++timer;
        stk.push_back(u);
        for (int v : g[u]) {
            if (!id[v]) { dfs(v); low[u] = min(low[u], low[v]); }
            else if (comp[v] == -1) low[u] = min(low[u], id[v]);
        }
        if (low[u] == id[u]) {
            int v;
            do { v = stk.back(); stk.pop_back(); comp[v] = scc; } while (u != v);
            scc++;
        }
    }

    bool solve() {
        for (int i = 0; i < 2 * n; ++i) if (!id[i]) dfs(i);
        for (int i = 0; i < n; ++i) {
            if (comp[T(i)] == comp[F(i)]) return false;
            ans[i] = comp[T(i)] < comp[F(i)];
        }
        return true;
    }
};
