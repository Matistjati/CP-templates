// why would you need this, just memorize smh
const int hi = int(2e5 + 10);
struct LCA
{
    LCA() {}
    LCA(vvi& edges)
    {
        dfs(0, 0, 0, edges);
    }

    int up[19][hi]; // TODO: dynamic lg
    int depth[hi];
    int tin[hi];
    int tout[hi];
    int timer = 0;
    void dfs(int u, int p, int d, vvi& edges)
    {
        up[0][u] = p;
        repp(d, 1, 19) up[d][u] = up[d-1][up[d - 1][u]];

        depth[u] = d;
        tin[u] = timer++;

        repe(e, edges[u])
        {
            if (e == p) continue;
            dfs(e, u, d + 1, edges);
        }
        tout[u] = timer++;
    }

    bool isancestor(int a, int b)
    {
        return tin[a] <= tin[b] && tout[a] >= tout[b];
    }

    int lca(int a, int b)
    {
        if (isancestor(a, b)) return a;
        if (isancestor(b, a)) return b;

        per(d, 19)
        {
            if (!isancestor(up[d][a], b)) a = up[d][a];
        }

        return up[0][a];
    }

    int dist(int a, int b)
    {
        int c = lca(a, b);
        return depth[a] + depth[b] - 2 * depth[c];
    }
};