struct Edmonds
{
	vvi flow;
	vvi cap;
	vi par;
	int n;
	int tot_flow = 0;

	void addEdge(int a, int b, int c)
	{
		cap[a][b] += c;
	}

	Edmonds(int n) : n(n), flow(n, vi(n)), cap(n, vi(n)), par(n) {}

	int bfs(int s, int t)
	{
		rep(i, n) par[i] = -1;
		par[s] = -2;
		queue<int> q;
		q.emplace(s);

		while (q.size())
		{
			int u = q.front(); q.pop();

			rep(e, n)
			{
				if (par[e] == -1 && flow[u][e] < cap[u][e])
				{
					par[e] = u;
					if (e == t) return 1;
					q.emplace(e);
				}
			}
		}

		return 0;
	}

	int augment(int s, int t) // find one augmenting path
	{
		if (bfs(s, t))
		{
			int f = inf;
			for (int cur = t; cur != s; cur = par[cur])
			{
				int p = par[cur];
				f = min(f, cap[p][cur]-flow[p][cur]);
			}
			tot_flow += f;
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

	int maxflow(int s, int t)
	{
		while (augment(s, t)) {}
		return tot_flow;
	}
};
