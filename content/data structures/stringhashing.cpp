typedef unsigned long long ull;

struct RollingHash
{
    vector<ull> suf, b;
    int mod;
    RollingHash(string& s, ull base = 131, ull mod = 1e9 + 7) : mod(mod), suf(s.size() + 1), b(s.size() + 1)
    {
        b[0] = 1;
        b[1] = base;

        for (int i = sz(s)-1; i >= 0; i--)
        {
            suf[i] = (suf[i + 1] * base + s[i]) % mod;
        }

        repp(i, 2, s.size() + 1)
        {
            b[i] = b[i - 1] * b[1] % mod;
        }
    }

    ull gethash(int l, int r) // hash s[l, r]
    {
        ull rv = (suf[r + 1] * b[r - l + 1]) % mod;
        return suf[l] + mod*(suf[l]<rv) - rv;
    }
};

struct DoubleHash
{
    RollingHash h1;
    RollingHash h2;
    DoubleHash(string& s, ull mod=1e9+7) : h1(s, 157, mod), h2(s, 131, mod) {}
    ull gethash(int l, int r)
    {
        return (h1.gethash(l, r) << 32) + h2.gethash(l, r);
    }
};

