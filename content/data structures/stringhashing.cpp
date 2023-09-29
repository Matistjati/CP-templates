typedef unsigned long long ull;
struct RollingHash
{
    vi suf, b;
    int mod;
    RollingHash(string& s, ull base = 131, ull mod = 1e9 + 7) : mod(mod), suf(s.size() + 1), b(s.size() + 1)
    {
        b[0] = 1;
        b[1] = base;

        per(i, s.size())
        {
            suf[i] = ((ll)suf[i + 1] * base + s[i]) % mod;
        }

        repp(i, 2, s.size() + 1)
        {
            b[i] = (ll)b[i - 1] * b[1] % mod;
        }
    }

    ull gethash(int l, int r) // get for s in interval [l, r]
    {
        ll v = suf[l] - (ll)suf[r + 1] * b[r - l + 1];
        // Make sure that v is within [0, mod)
        v %= mod;
        v += mod;
        v %= mod;

        return v;
    }
};

struct DoubleHash
{
    RollingHash h1;
    RollingHash h2;
    DoubleHash(string& s) : h1(s, 157, mod), h2(s, 131, mod) {}
    ull gethash(int l, int r)
    {
        ull hash = ((ull)h1.gethash(l, r) << 32) + h2.gethash(l, r);
        return hash;
    }
};

ull stringHash(const string& s)
{
    int pow = 153;
    ull ret = 0;
    ull base = 1;

    rep(i, s.size())
    {
        ret += s[i] * base;
        base *= pow;
    }
    return ret;
}

vector<ull> prefixHash(string& s)
{
    int pow = 153;
    ull ret = 0;
    ull base = 1;
    vector<ull> prefixes(s.size());

    rep(i, s.size())
    {
        ret += s[i] * base;
        prefixes[i] = ret;
        base *= pow;
    }
    return prefixes;
}