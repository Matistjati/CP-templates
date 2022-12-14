
// Helper function
void setDivisors(ull n, ull i, vector<ull>& divisors, vector<pair<ull, ull>>& factors) {
    ull j, x, k;
    for (j = i; j < factors.size(); j++) {
        x = factors[j].first * n;
        for (k = 0; k < factors[j].second; k++) {
            divisors.push_back(x);
            setDivisors(x, j + 1, divisors, factors);
            x *= factors[j].first;
        }
    }
}
// Get all factor from a list of primes
vector<ull> getDivisors(vector<ull> primes)
{
    unordered_map<ull, ull> primeCount;
    repe(prime, primes) primeCount[prime]++;
    vector<pair<ull, ull>> factorCount;
    repe(p, primeCount) factorCount.emplace_back(p);
    vector<ull> ret;
    setDivisors(1, 0, ret, factorCount);
    ret.push_back(1);
    return ret;
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