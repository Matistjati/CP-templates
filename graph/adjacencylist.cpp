/*
Faster adjacency list
*/
using edge = int;
struct AdjacencyList // directed adjacency list
{
    vector<int> outdeg, counter;
    vector<edge> adj;
    AdjacencyList(int n, vector<pair<int, edge>>& edges) : outdeg(n), adj(sz(edges)) {
        for (auto& [from, e] : edges) {
            outdeg[from]++;
        }
        outdeg.insert(outdeg.begin(), 0);
        repp(i, 1, sz(outdeg)) outdeg[i] += outdeg[i - 1];
        counter = outdeg;
        for (auto& [from, e] : edges) {
            adj[counter[from]++] = e;
        }
    }

    span<edge> operator[](int u) {
        return span<edge>(adj.data() + outdeg[u], adj.data() + outdeg[u+1]);
    }
};
