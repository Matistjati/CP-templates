vector<ull> getDivisors(vector<ull> primes) {
    unordered_map<ull, int> cnt;
    repe(p, primes) cnt[p]++;
    vector<ull> d = { 1 };
    for (auto [p, e] : cnt) {
        int n0 = sz(d);
        rep(i, n0) {
            ll cur = d[i];
            rep(j, e) {
                cur *= p;
                d.push_back(cur);
            }
        }
    }
    sort(all(d));
    return d;
}

// Get the number of divisors given factors. A lot faster than getDivisors(...).size()
ull getDivisorcount(vector<ull> factors, bool isSorted = false)
{
    if (!isSorted) sort(all(factors));
    if (factors.size() == 0) return 1;

    ull ans = 1;
    ull f = factors[0];
    int i = 0;
    while (i < factors.size())
    {
        int start = factors[i];
        int cnt = 0;
        while (i < factors.size() && factors[i] == start)
        {
            cnt++;
            i++;
        }
        ans *= cnt + 1;
    }
    return ans;
}
// Get a list of the prime factors in the form (base, power). e.g. 2^5+11^2
typedef pair<ull, ull> pull;
typedef vector<pull> vpll;
vpll power_list(vector<ull> factors)
{
    vpll ret;
    sort(all(factors));
    int i = 0;
    while (i < factors.size())
    {
        int start = factors[i];
        pull prime;
        prime.first = start;
        for (; i < factors.size() && factors[i] == start; i++)
        {
            prime.second++;
        }
        ret.push_back(prime);
    }
    return ret;
}

int phi(vector<ull> primefactors)
{
    vpll primes = power_list(primefactors);
    int ans = 1;
    repe(p, primes) ans *= binpow(p.first, p.second - 1) * (p.first - 1);
    return ans;
}

// Find all divisors in sqrt(a) time. pretty bad
vi getDivsSlow(ll a)
{
    vi ret;
    int i;
    for (i = 1; i * i < a; i++) {
        if (a % i == 0)
            ret.push_back(i);
    }
    if (i - (a / i) == 1) {
        i--;
    }
    for (; i >= 1; i--) {
        if (a % i == 0)
            ret.push_back(a / i);
    }
    return ret;
}
