struct UF {
    vector<int> p;
    UF(int n) : p(n) { iota(all(p), 0); }
    int find(int x) {
        while (p[x] != x) x = p[x] = p[p[x]];
        return x;
    }
    bool same(int x, int y) {
        for (;;) {
            int px = p[x], py = p[y];
            if (px == py) return true;
            if (px < py) swap(x, y), swap(px, py);
            if (x == px) return false;
            x = p[x] = p[px];
        }
    }
    bool join(int x, int y) {
        for (;;) {
            int px = p[x], py = p[y];
            if (px == py) return false;
            bool sw = px < py;
            int nx = sw ? y : x, ny = sw ? x : y, pnx = sw ? py : px, pny = sw ? px : py;
            p[nx] = pny;
            if (nx == pnx) return true;
            x = pnx, y = ny;
        }
    }
};
