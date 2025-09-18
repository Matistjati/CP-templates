
const int maxn = 100;
typedef vector<bitset<maxn>> bset;
const double time_lim_ms = 50;
vi MaxClique(bset adj)
{
    int n = sz(adj);
    mt19937_64 rng(10);
    uniform_real_distribution<double> dist01(0, 1);
    uniform_int_distribution<int> node_dist(0, n - 1);

    bitset<maxn> cur;
    int best_score = 0;
    int cur_score = 0;

    const double Tlo = 1e-3;
    const double Thi = 100;

    auto start = chrono::high_resolution_clock::now();
    bitset<maxn> ret;
    double t, T;
    int i = 0;
    while (1)
    {
        if (i++%100==0)
        {
            t = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() / time_lim_ms;
            if (t > 0.99) break;
            T = Tlo * pow(Thi / Tlo, t);
        }

        int u = node_dist(rng);
        int increase = 0;
        if (!cur[u])
        {
            increase = 1 - (~adj[u] & cur).count();
        }
        else increase = -1;

        if (increase >= 0 || dist01(rng) < exp(increase / T))
        {
            if (!cur[u]) cur = (cur & adj[u]);
            cur[u] = !cur[u];
            cur_score += increase;
            if (cur_score > best_score)
            {
                best_score = cur_score;
                ret = cur;
            }
            best_score = max(best_score, cur_score);
        }
    }
    vi ret_ind;
    rep(i, n) if (ret[i]) ret_ind.push_back(i);
    return ret_ind;
}
vi MaxIndepSet(bset adj)
{
    rep(i, sz(adj)) adj[i] = ~adj[i], adj[i][i] = 0;
    return MaxClique(adj);
}
vi MinVertexCover(bset adj)
{
    vi ret;
    vi indep = MaxIndepSet(adj);
    rep(i, sz(adj)) if (!binary_search(all(indep), i)) ret.push_back(i);
    return ret;
}

