// Status: all operations but / are tested
ll euclid(ll a, ll b, ll& x, ll& y) {
    if (!b) return x = 1, y = 0, a;
    ll d = euclid(b, a % b, y, x);
    return y -= a / b * x, d;
}
template<int mod>
struct Mint {
    ll x;
    Mint(ll xx) : x(xx) { x %= mod; x += mod; x %= mod; }
    // No-mod constructor
    Mint(ll xx, bool nope) : x(xx) { }
    Mint() : x(0) {}
    Mint operator+(Mint b) { int y = x + b.x; return { y - (y >= mod) * mod, false }; }
    Mint operator-(Mint b) { int y = x - b.x; return { y + (y < 0) * mod, false }; }
    Mint operator-() { return { -x+mod, false }; }
    Mint operator*(Mint b) { return { x * b.x % mod, false }; }
    Mint operator/(Mint b) { return { x * invert(b) % mod, false }; }
    Mint invert(Mint a) {
        ll x, y, g = euclid(a.x, mod, x, y);
        assert(g == 1); return { (x + mod) % mod };
    }
};
typedef Mint<int(1e9 + 7)> Mod;
