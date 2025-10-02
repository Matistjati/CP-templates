/**
 * Author: Joshua Andersson
 * License: CC0
 * Source: folklore
 * Description: Stores the upper hull of a bunch of lines.
 * Implements query max at some coordinate, max plus convolution/
 * minkowski sum, and max of two. Unclear numerical stability.
 */

struct Line {
    long long k, m; // y = kx + m
    mutable long double xRight; // intersection with next line
    bool operator<(const Line& o) const {
        return k < o.k;
    }
    Line operator+(const Line& other) const {
        return { k + other.k,m + other.m, 0 };
    }
};

long double intersect(const Line& a, const Line& b) {
    return (long double)(b.m - a.m) / (a.k - b.k);
}

const ll maxt = ll(8e9) + 10;
struct UpperHull {
    vector<Line> hull;

    static vector<Line> buildHull(vector<Line>& lines) {
        sort(all(lines));
        vector<Line> res;
        res.reserve(sz(lines));
        for (const Line& l : lines) {
            if (!res.empty() && res.back().k == l.k) {
                if (res.back().m >= l.m) continue;
                res.pop_back();
            }
            while (res.size() >= 2 &&
                intersect(res[sz(res)-2], res.back()) >= intersect(res[sz(res) - 2], l)) {
                res.pop_back();
            }
            res.push_back(l);
        }

        rep(i, sz(res)-1) res[i].xRight = intersect(res[i], res[i + 1]);
        if (!res.empty()) res.back().xRight = 1e18;
        return res;
    }

    UpperHull(const vector<Line>& lines) : hull(lines) {}
    UpperHull(vector<p2>& input) {
        vector<Line> temp;
        for (auto& [k, m] : input) temp.push_back({k, m, 0 });
        hull = buildHull(temp);
    }
    UpperHull() {}

    UpperHull max(const UpperHull& other) const {
        vector<Line> merged = hull;
        merged.insert(merged.end(), all(other.hull));
        return UpperHull{ buildHull(merged) };
    }

    UpperHull minkowskiSum(const UpperHull& other) const {
        auto& A = hull;
        auto& B = other.hull;
        size_t i = 0, j = 0;
        vector<Line> merged;
        while (i < A.size() && j < B.size()) {
            merged.push_back(A[i] + B[j]);
            long double x1 = (i + 1 < A.size() ? A[i].xRight : LDBL_MAX);
            long double x2 = (j + 1 < B.size() ? B[j].xRight : LDBL_MAX);
            if (x1 < x2)        ++i;
            else if (x2 < x1)   ++j;
            else { ++i; ++j; }
        }
        while (i < A.size()) merged.push_back(A[i++] + B.back());
        while (j < B.size()) merged.push_back(A.back() + B[j++]);
        return UpperHull{ buildHull(merged) };
    }

    long double eval(long double x) const {
        auto it = lower_bound(all(hull), x,
            [](const Line& L, long double v) {
                return L.xRight < v;
            });
        if (it == hull.end()) it = prev(it);
        return (long double)it->k * x + it->m;
    }
};

