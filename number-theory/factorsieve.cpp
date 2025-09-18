const int MAX_PR = 1e5;
vi smallestfactor(MAX_PR);

// Find smallest factor of all numbers in range[0, hi)
void smallestFactorSieve(int hi = MAX_PR)
{
    rep(i, hi) smallestfactor[i] = i;
    for (int i = 0; i < hi; i += 2) smallestfactor[i] = 2;

    repp(i, 3, min(hi + 0., ceil(sqrt(hi)) + 10))
    {
        if (smallestfactor[i] == i)
        {
            for (size_t j = i * i; j < hi; j += i)
            {
                if (smallestfactor[j] == j) smallestfactor[j] = i;
            }
        }
    }
}
// Factor n in log(n) time
vector<ull> GetFactors(int n)
{
    vector<ull> factors;
    int k = n;
    while (k != 1 && k != 0)
    {
        factors.emplace_back(smallestfactor[k]);
        k /= smallestfactor[k];
    }
    return factors;
}