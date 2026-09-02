struct BitsetKuhn {
	using ull = unsigned long long;
	int n, m, W;
	vector<ull> adj, vis;
	vector<int> mtA, mtB, ord, perm, inv;

	BitsetKuhn(int n_, int m_)
		: n(n_), m(m_), W((m_ + 63) / 64), adj(size_t(n_)* W), vis(W),
		mtA(n_, -1), mtB(m_, -1), ord(n_), perm(m_), inv(m_) {
		mt19937 rng(42);
		iota(ord.begin(), ord.end(), 0);
		iota(perm.begin(), perm.end(), 0);
		shuffle(ord.begin(), ord.end(), rng);
		shuffle(perm.begin(), perm.end(), rng);
		for (int v = 0; v < m; v++) inv[perm[v]] = v;
	}

	void add(int u, int v) {
		int p = perm[v];
		adj[size_t(u) * W + p / 64] |= 1ull << (p & 63);
	}

	bool aug(int u) {
		const ull* a = &adj[size_t(u) * W];
		for (int w = 0; w < W; w++)
			for (ull cur = a[w] & vis[w]; cur; cur &= vis[w]) {
				int v = w * 64 + countr_zero(cur);
				vis[w] ^= 1ull << (v & 63);
				if (mtB[v] < 0 || aug(mtB[v])) return mtB[v] = u, mtA[u] = v, true;
			}
		return false;
	}

	int solve() { // size of matching
		int res = 0;
		ranges::fill(vis, ~0ull);
		for (int u : ord) {
			const ull* a = &adj[size_t(u) * W];
			for (int w = 0; w < W; w++) if (ull c = a[w] & vis[w]) {
				int v = w * 64 + countr_zero(c);
				vis[w] ^= 1ull << (v & 63);
				mtA[u] = v, mtB[v] = u, res++;
				break;
			}
		}
		for (int u : ord) if (mtA[u] < 0) {
			ranges::fill(vis, ~0ull);
			res += aug(u);
		}
		return res;
	}

	int matchL(int u) const { return mtA[u] < 0 ? -1 : inv[mtA[u]]; } // right partner of u
	int matchR(int v) const { return mtB[perm[v]]; } // left partner of v
};
