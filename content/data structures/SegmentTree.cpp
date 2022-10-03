struct Tree
{
    int n;
    vi tree;
    Tree(int n) : n(n), tree(n * 4, -inf) {}

    int merge(int a, int b)
    {
        return max(a, b);
    }

    void update(int x, int l, int r, int i, int v)
    {
        if (l == r) tree[x] = v;
        else
        {
            int mid = (l + r) / 2;
            if (i <= mid) update(x * 2, l, mid, i, v);
            else update(x * 2 + 1, mid + 1, r, i, v);
            tree[x] = merge(tree[x * 2], tree[x * 2 + 1]);
        }
    }

    void update(int i, int v) { update(1, 0, n - 1, i, v); }

    int query(int x, int l, int r, int ql, int qr)
    {
        if (l > qr || r < ql) return -inf;
        if (l >= ql && r <= qr) return tree[x];

        int mid = (l + r) / 2;
        return merge(query(x * 2, l, mid, ql, qr), query(x * 2 + 1, mid + 1, r, ql, qr));
    }

    int query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }
};