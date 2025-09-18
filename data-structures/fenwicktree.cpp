struct Tree
{
    vi tree;
    Tree(int n) : tree(n+1) {}

    void update(int i, int v)
    {
        for (i++; i < tree.size(); i += i & -i)
            tree[i] += v;
    }

    int query(int r)
    {
        int ret = 0;
        for (r++; r > 0; r -= r & -r)
            ret += tree[r];
        return ret;
    }

    int sum(int l, int r)
    {
        if (l) l = query(l-1);
        return query(r) - l;
    }
};
