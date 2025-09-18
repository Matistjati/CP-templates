struct UF
{
    int n;
    vi parent;
    vi size;

    UF(int n) : n(n), parent(n), size(n, 1)
    {
        rep(i, n) parent[i] = i;
    }

    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }

    void merge(int a, int b)
    {
        a = find(a); b = find(b);
        if (a == b) return;

        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
    }
};