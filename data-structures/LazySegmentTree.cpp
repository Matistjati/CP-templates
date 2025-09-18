struct LazyTree // Range add, range max query
{
    const int unit = 0;
    int n;
    vi tree;
    vi lazy;
    LazyTree(int n) : n(n), tree(n * 4, unit), lazy(n * 4, unit) {}

    int merge(int a, int b)
    {
        return max(a, b);
    }

    void push(int x)
    {
        tree[x * 2] += lazy[x];
        tree[x * 2 + 1] += lazy[x];
        lazy[x * 2] += lazy[x];
        lazy[x * 2 + 1] += lazy[x];
        lazy[x] = 0;
    }

    void add(int x, int l, int r, int ql, int qr, int v)
    {
        if (l > qr || r < ql) return;
        if (l >= ql && r <= qr)
        {
            lazy[x] += v;
            tree[x] += v;
            return;
        }
        push(x);

        int mid = (l + r) / 2;
        add(x * 2, l, mid, ql, qr, v);
        add(x * 2 + 1, mid + 1, r, ql, qr, v);
        tree[x] = merge(tree[x * 2], tree[x * 2 + 1]);
    }

    void add(int ql, int qr, int v) { add(1, 0, n - 1, ql, qr, v); }

    int query(int x, int l, int r, int ql, int qr)
    {
        if (l > qr || r < ql) return -inf;
        if (l >= ql && r <= qr) return tree[x];
        push(x);

        int mid = (l + r) / 2;
        return merge(query(x * 2, l, mid, ql, qr), query(x * 2 + 1, mid + 1, r, ql, qr));
    }

    int query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }
};
