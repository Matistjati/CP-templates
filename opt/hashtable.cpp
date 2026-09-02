
typedef uint64_t ull;
ull hsh(ll x) {
  auto inp = _mm_set1_epi64x(x);
  auto key = _mm_set1_epi64x(18246162); // any key
  auto out = _mm_aesdec_si128(inp, key);
  return _mm_extract_epi64(out, 0);
}
const ull EMPTY = 128373173617322; // forbidden x
template<class T>
struct HashMap {
  int b;
  vector<pair<ull, T>> v;
  HashMap(int b, T defval) : b(b), v(1<<b, {EMPTY, defval}) {}
  T& operator[](ull x) {
    ull y = hsh(x) >> (64 - b), m = (1<<b) - 1;
    while (v[y].first != EMPTY && v[y].first != x) ++y &= m;
    // return v[y].first == x; // .count()
    v[y].first = x;
    return v[y].second;
  }
};
