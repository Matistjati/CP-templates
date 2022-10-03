// Find the largest k such that n!%p^k==0
ll largestPower(ll n, ll p)
{
    // Initialize result
    ll k = LLONG_MAX;

    map<ll, int> factorCount;
    repe(f, factor(p)) factorCount[f]++;
    repe(f, factorCount)
    {
        ll fact = n;
        ll ans = 0;
        while (fact)
        {
            fact /= f.first;
            ans += fact;
        }
        k = min(k, ans / f.second);
    }

    return k;
}
