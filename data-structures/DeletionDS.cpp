/**
 * Description: Given a data structure (DS) with O(T(n))
 * non-amortized insertion time, add deletes. Will call
 * fn(time, ds): the data structure after the first time+1
 * updates have been applied. Your responsibility to find the
 * right queries. Take care if queries come before any updates
 * and to normalize items: (a,b)!=(b,a)
 * Status: tested on omogenjudge:Dynamic Connectivity
 */

template<typename DS, typename Item>
struct QueryTree {
    vector<vector<Item>> items;
    int T;

    QueryTree(vector<pair<char,Item>> updates) : T(sz(updates)) {
        items.resize(4 * T);
        map<Item, int> alive;
        int i = 0;
        for (auto [t, item] : updates) {
            if (t=='+') {
                if (!alive.count(item)) alive[item] = i;
            }
            else {
                assert(alive.count(item));
                add_item(item, alive[item], i - 1);
                alive.erase(item);
            }
            i++;
        }
        for (auto [item, t] : alive) {
            add_item(item, t, T - 1);
        }
    }

    void add_to_tree(int x, int l, int r, int ql, int qr, const Item& q) {
        if (l > qr || r < ql) return;
        if (l >= ql && r <= qr) {
            items[x].push_back(q);
            return;
        }
        int mid = (l + r) / 2;
        add_to_tree(2 * x, l, mid, ql, qr, q);
        add_to_tree(2 * x + 1, mid + 1, r, ql, qr, q);
    }

    void add_item(const Item& q, int l, int r) {
        add_to_tree(1, 0, T - 1, l, r, q);
    }

    void dfs(int x, int l, int r, DS& ds, const function<void(int, DS&)>& fn) {
        for (Item& q : items[x]) {
            ds.add(q);
        }
        if (l == r) fn(l, ds);
        else {
            int mid = (l + r) / 2;
            dfs(2 * x, l, mid, ds, fn);
            dfs(2 * x + 1, mid + 1, r, ds, fn);
        }
        rep(i, sz(items[x])) ds.undo();
    }

    void run(DS& ds, const function<void(int, DS&)>& fn) {
        dfs(1, 0, T - 1, ds, fn);
    }
};


///// Driver code
// With union-find, we get dynamic connectivity

struct Edge {
    int a, b;
    Edge(int a, int b) : a(min(a,b)), b(max(a,b)) {}
    bool operator<(const Edge& other) const {
        if (a != other.a) return a < other.a;
        return b < other.b;
    }
};

struct state {
    ll a, b, parb, sizea;
};

struct UF {
    vector<state> history;
    vi par, size;
    UF(int n) : par(n), size(n, 1) {
        rep(i, n)par[i] = i;
    }
    int find(int x) { return par[x] == x ? x : find(par[x]); }
    void add(Edge e) {
        int a = find(e.a), b = find(e.b);
        if (size[a] < size[b]) swap(a, b);
        history.push_back({ a,b,par[b],size[a] });
        if (a!=b){
            par[b] = a;
            size[a] += size[b];
        }
    }
    void undo() {
        auto [a, b, parb, sizea] = history.back();
        history.pop_back();
        par[b] = parb;
        size[a] = sizea;
    }
};


int main() {
    int n, q;
    cin >> n >> q;
    vector<pair<char, Edge>> changes;
    changes.emplace_back('+', Edge{ 0,0 });
    vector<vector<p2>> query_at(q);
    rep(i, q)
    {
        int t, a, b;
        cin >> t >> a >> b;
        if (t==0)
        {
            changes.emplace_back('+', Edge{ a,b });
        }
        else if (t==1)
        {
            changes.emplace_back('-', Edge{ a,b });
        }
        else
        {
            query_at[sz(changes) - 1].emplace_back(a, b);
        }
    }
    QueryTree<UF, Edge> queries(changes);
    UF uf(n);
    queries.run(uf, [&query_at](int i, UF& uf)
        {
            for (auto [a, b] : query_at[i]) cout << (uf.find(a) == uf.find(b) ? "Ja" : "Nej") << '\n';
        });

    return 0;
}
