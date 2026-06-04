using item = int;
struct CSR
{
    vector<pair<int, item>> pending;
    vector<int> outdeg, counter;
    vector<item> adj;
    // N: number of rows. M: total number of items
    CSR(int n, int m) : adj(m), outdeg(n) { pending.reserve(m); }
    CSR() { }
    void add(int idx, const item& x) { pending.emplace_back(idx, x); }
    void build() {
        assert(sz(pending)==sz(adj));
        for (auto& [from, e] : pending) {
            outdeg[from]++;
        }
        outdeg.insert(outdeg.begin(), 0);
        partial_sum(all(outdeg), begin(outdeg));
        counter = outdeg;
        for (auto& [from, e] : pending) {
            adj[counter[from]++] = e;
        }
        pending.clear();
    }

    span<item> operator[](int u) {
        assert(pending.empty());
        return span<item>(adj.data() + outdeg[u], adj.data() + outdeg[u+1]);
    }
};

