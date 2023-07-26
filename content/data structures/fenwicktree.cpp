struct Tree
{
	vi s;
	Tree(int n) : s(n) {}

	void update(int i, int v)
	{
		for (; i < sz(s); i |= i + 1) s[i] += v;
	}

	int query(int r)
	{
		int ret = 0;
		for (; r; r &= r - 1) ret += s[r - 1];
		return ret;
	}

	int query(int l, int r) { return query(r + 1) - query(l); }
};
