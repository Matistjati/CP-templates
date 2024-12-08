struct Centroid {
    vector<vi> adj, children;
    vi cnt, par;
    Centroid(const vector<vi>& adj) : adj(adj),
        children(sz(adj)), cnt(sz(adj),-1), par(sz(adj),-1) {
        dfs(0, -1);
    }
    void calc_sz(int u, int p) {
        cnt[u] = 1;
        for (int e : adj[u])
            if (e != p) calc_sz(e, u), cnt[u] += cnt[e];
    }
    int find_centroid(int u, int p, int n) {
        for (int e : adj[u])
            if (e != p && cnt[e] > n / 2)
                return find_centroid(e, u, n);
        return u;
    }
    void dfs(int u, int p) {
        calc_sz(u, -1);
        u = find_centroid(u, u, cnt[u]);
        par[u] = p;
        if (p!=-1) children[p].push_back(u);
        for (int e : adj[u]) {
            adj[e].erase(find(all(adj[e]), u));
            dfs(e, u);
        }
    }
};
