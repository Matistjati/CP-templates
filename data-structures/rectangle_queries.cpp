struct Segtree
{
    int n;
    vi tree;
    vi realindex;

    Segtree() {}

    Segtree(vi nums)
    {

        map<int, int> occs;
        rep(i, nums.size()) occs[nums[i]]++;
        n = occs.size();
        tree = vi(n * 4);
        int i = 0;
        repe(num, occs)
        {
            realindex.push_back(num.first);
            update(i, num.second);
            i++;
        }
    }

    void update(int x, int l, int r, int i, int v)
    {
        if (l > i || r < i) return;
        if (l == r) tree[x] += v;
        else
        {
            int mid = (l + r) / 2;
            update(x * 2, l, mid, i, v);
            update(x * 2 + 1, mid + 1, r, i, v);
            tree[x] = tree[x * 2] + tree[x * 2 + 1];
        }
    }
    void update(int i, int v) { update(1, 0, n - 1, i, v); }

    int query(int x, int l, int r, int ql, int qr)
    {
        if (l > qr || r < ql) return 0;
        if (l >= ql && r <= qr) return tree[x];
        int mid = (l + r) / 2;
        return query(x * 2, l, mid, ql, qr) + query(x * 2 + 1, mid + 1, r, ql, qr);
    }

    int query(int ql, int qr)
    {
        ql = lower_bound(all(realindex), ql) - realindex.begin();
        qr = upper_bound(all(realindex), qr) - realindex.begin() - 1;
        return query(1, 0, n - 1, ql, qr);
    }
};

struct Tree2D
{
    int n;
    vector<Segtree> trees;
    vvi points;
    vi realindex;

    Tree2D(vp2 points)
    {
        map<int, vi> rows;
        repe(p, points)
        {
            rows[p.first].push_back(p.second);
        }

        repe(num, rows)
        {
            realindex.push_back(num.first);
        }

        n = rows.size();
        trees = vector<Segtree>(n * 4);
        this->points = vvi(n * 4);

        vvi values;
        repe(row, rows) values.push_back(row.second);
        build(1, 0, n - 1, values);
    }

    void build(int x, int l, int r, vvi& values)
    {
        if (l == r)
        {
            points[x] = values[l];
            trees[x] = Segtree(values[l]);
        }
        else
        {
            int mid = (l + r) / 2;
            build(x * 2, l, mid, values);
            build(x * 2 + 1, mid + 1, r, values);

            repe(p, points[x * 2]) points[x].push_back(p);
            repe(p, points[x * 2 + 1]) points[x].push_back(p);
            trees[x] = Segtree(points[x]);
        }
    }

    int query(int x, int l, int r, int ql, int qr, int xl, int xr)
    {
        if (l > qr || r < ql) return 0;
        if (l >= ql && r <= qr) return trees[x].query(xl, xr);
        int mid = (l + r) / 2;
        return query(x * 2, l, mid, ql, qr, xl, xr) + query(x * 2 + 1, mid + 1, r, ql, qr, xl, xr);
    }

    int query(int ql, int qr, int xr, int xl)
    {
        ql = lower_bound(all(realindex), ql) - realindex.begin();
        qr = upper_bound(all(realindex), qr) - realindex.begin() - 1;
        return query(1, 0, n - 1, ql, qr, xr, xl);
    }
};
