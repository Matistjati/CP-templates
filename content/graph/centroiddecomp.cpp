struct Centroid
{
    int n;
    vvi edges;
    vb vis;
    vi par;
    vi size;

    Centroid() {}
    Centroid(vvi& edges) : edges(edges), n(edges.size()), vis(n),par(n),size(n)
    {
        init_centroid(0, -1);
    }

    int find_centroid(int u, int p, int n)
    {
        repe(e, edges[u])
        {
            if (e == p) continue;
            if (!vis[e] && size[e] > n / 2) return find_centroid(e, u, n);
        }
        return u;
    }

    int find_size(int u, int p)
    {
        if (vis[u]) return 0;
        size[u] = 1;

        repe(e, edges[u])
        {
            if (e == p) continue;
            size[u] += find_size(e, u);
        }
        return size[u];
    }

    void init_centroid(int u, int p)
    {
        find_size(u, u);

        int c = find_centroid(u, u, size[u]);
        vis[c] = 1;
        par[c] = p;

        repe(e, edges[c])
        {
            if (!vis[e]) init_centroid(e, c);
        }
    }
};