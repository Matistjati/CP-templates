// Precompute log(x!). log(n choose k)=exp(log(n!)-log(k!)-log((n-k)!))
// Useful when computing ratios, like (n choose k)/(m choose x)=exp(choose(n,k)-choose(m,x))
struct LogFactorial
{
    vector<double> fact;
    LogFactorial(int hi) : fact(hi, 0)
    {
        double v = 0;
        repp(i, 1, hi)
        {
            v += log(i);
            fact[i] = v;
        }
    }
    inline double operator[](int index) { return fact[index]; }
    inline double choose(int n, int k) { return fact[n] - fact[k] - fact[n - k]; }
};

struct ModFactorial
{
    vi fact;
    ModFactorial(int hi, int mod) : fact(hi, 0)
    {
        int v = 1;
        repp(i, 1, hi)
        {
            v *= i;
            v %= mod;
            fact[i] = v;
        }
    }
    inline double operator[](int index) { return fact[index]; }
};