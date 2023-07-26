struct Tree
{
	vvi s;
	Tree(int n) : s(n, vi(n)) {}

	void update(int i, int j, int v)
	{
		for (; i < sz(s); i |= i + 1)
			for (int k = j; k < sz(s); k |= k + 1)
				s[i][k] += v;
	}

	int query(int r, int c)
	{
		int ret = 0;
		for (; r; r &= r - 1)
			for (int k = c; k; k &= k - 1)
				ret += s[r - 1][k - 1];
		return ret;
	}

	int query(int a, int b, int c, int d)
	{
		return query(b + 1, d + 1) - query(a, d + 1) - query(b + 1, c) + query(a, c);
	}
};
