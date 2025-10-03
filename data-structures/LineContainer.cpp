/**
 * Author: Joshua Andersson
 * License: CC0
 * Source: folklore
 * Description: Stores the upper hull of a bunch of lines.
 * Implements query max at some coordinate, max plus convolution/
 * minkowski sum, and max of two. Unclear numerical stability.
 * Status: tested on kattis:Ruler of Everything and swerc24:titanmachy
 */

 struct Line {
    ll k, m, xRight; // y = kx + m and intersection with next line
    bool operator<(const Line& o) const {
        return k < o.k;
    }
    bool operator<(ll x) const {
        return xRight < x;
    }
    Line operator+(const Line& other) const {
        return { k + other.k,m + other.m, 0 };
    }
    ll eval(ll x) const {
        return k * x + m;
    }
};

#if _LOCAL
#include <__msvc_int128.hpp>
using big_signed = _Signed128;
#else
using big_signed = __int128;
#endif

struct UpperHull { // invariant: every upperhull is sorted and no useless lines
    // if you add custom operations, ensure xRight is always set
    vector<Line> hull;
    static ll div(ll a, ll b) { // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }
    static bool intersection_geq(const Line& A, const Line& B, const Line& C) {
        big_signed left = (big_signed)(B.m - A.m) * (big_signed)(A.k - C.k);
        big_signed right = (big_signed)(C.m - A.m) * (big_signed)(A.k - B.k);
        return left >= right;
    }
    static vector<Line> merge_sorted(const vector<Line>& a, const vector<Line>& b) {
        vector<Line> res;
        res.reserve(sz(a) + sz(b));
        int i = 0;
        int j = 0;
        auto tryadd = [&](const Line& l) {
            if (!res.empty() && res.back().k == l.k) {
                if (res.back().m >= l.m) return;
                res.pop_back();
            }
            while (res.size() >= 2 && intersection_geq(res[sz(res) - 2], res.back(), l)) {
                res.pop_back();
            }
            res.push_back(l);
            };
        while (i < sz(a) && j < sz(b)) {
            if (a[i].k < b[j].k) tryadd(a[i++]);
            else tryadd(b[j++]);
        }
        while (i < sz(a)) tryadd(a[i++]);
        while (j < sz(b)) tryadd(b[j++]);
        rep(i, sz(res) - 1) res[i].xRight = div(res[i + 1].m - res[i].m, res[i].k - res[i + 1].k);
        if (!res.empty()) res.back().xRight = 1e18;
        return res;
    }

    static vector<Line> build_unsorted(vector<Line>& l) {
        sort(all(l));
        return merge_sorted(l, {});
    }

    UpperHull(vector<Line>& lines) : hull(build_unsorted(lines)) {}
    UpperHull(const vector<Line>& lines, bool) : hull(lines) {}
    UpperHull() {}

    UpperHull max(const UpperHull& other) const {
        return { merge_sorted(this->hull, other.hull), 0 };
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
        return { merge_sorted(merged, {}), 0 };
    }

    ll eval(ll x) const { // if assert fails, you didnt maintain invariants
        assert(sz(hull) && hull.back().xRight == inf);
        auto it = lower_bound(all(hull), x);
        return it->eval(x);
    }
};
