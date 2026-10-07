
#if _LOCAL
#include <__msvc_int128.hpp>
ull modmul(ull a, ull b, ull c) {
    return (ull)(_Unsigned128(a) * b % c);
}
#else
ull modmul(ull a, ull b, ull M) {
    ll ret = a * b - M * ull(1.L / M * a * b);
    return ret + M * (ret < 0) - M * (ret >= (ll)M);
}
#endif
// b^e % mod
ull modpow(ull b, ull e, ull mod) {
    ull ans = 1;
    for (; e; b = modmul(b, b, mod), e /= 2)
        if (e & 1) ans = modmul(ans, b, mod);
    return ans;
}
bool isPrime(ull n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    ull A[] = { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 },
        s = ctz(n - 1), d = n >> s;
    for (ull a : A) {   // ^ count trailing zeroes
        ull p = modpow(a % n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--)
            p = modmul(p, p, n);
        if (p != n - 1 && i != s) return 0;
    }
    return 1;
}
ull pollard(ull n) {
    auto f = [n](ull x) { return modmul(x, x, n) + 1; };
    ull x = 0, y = 0, t = 30, prd = 2, i = 1, q;
    while (t++ % 40 || gcd(prd, n) == 1) {
        if (x == y) x = ++i, y = f(x);
        if ((q = modmul(prd, max(x, y) - min(x, y), n))) prd = q;
        x = f(x), y = f(f(y));
    }
    return gcd(prd, n);
}
vector<ull> factor(ull n) {
    if (n == 1) return {};
    if (isPrime(n)) return { n };
    ull x = pollard(n);
    auto l = factor(x), r = factor(n / x);
    l.insert(l.end(), all(r));
    return l;
}

vector<ull> getDivisors(ull x) {
    vector<ull> pr = factor(x);
    sort(all(pr));
    vector<ull> d = {1};
    for (int i = 0, j; i < sz(pr); i = j) {
        for (j = i; j < sz(pr) && pr[j] == pr[i]; j++);
        int n0 = sz(d);
        ull pw = 1;
        rep(k, j - i) {
            pw *= pr[i];
            rep(t, n0) d.push_back(d[t] * pw);
        }
    }
    sort(all(d));
    return d;
}
