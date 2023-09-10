struct Edmonds
{
    vvi flow;
    vvi capacity;
    vi par;
    int n;
    int maxflow = 0;

    void addEdge(int a, int b, int c)
    {
        capacity[a][b] = c;
    }

    Edmonds(int n) : n(n), flow(n, vi(n)), capacity(n, vi(n)), par(n) {}

    int bfs(int s, int t)
    {
        rep(i, n) par[i] = -1;
        par[s] = -2;
        queue<p2> q;
        q.emplace(s, inf);

        while (q.size())
        {
            int u, f;
            tie(u, f) = q.front(); q.pop();

            rep(e, n)
            {
                if (par[e] == -1 && flow[u][e] < capacity[u][e])
                {
                    par[e] = u;
                    int newflow = min(f, capacity[u][e] - flow[u][e]);

                    if (e == t) return newflow;
                    q.emplace(e, newflow);
                }
            }
        }

        return 0;
    }

    int augment(int s, int t) // find one augmenting path
    {
        int f = 0;
        if (f = bfs(s, t))
        {
            maxflow += f;
            for (int cur = t; cur != s; cur = par[cur])
            {
                int p = par[cur];
                flow[p][cur] += f;
                flow[cur][p] -= f;
            }

            return 1;
        }
        return 0;
    }
};
