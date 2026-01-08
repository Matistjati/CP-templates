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
	ll k, m, xRight; // y = kx + m, and intersection with next line
	bool operator<(const Line& o) const {
		return k < o.k;
	}
	bool operator<(ll x) const {
		return xRight < x;
	}
	Line operator+(const Line& other) const {
		return { k + other.k,m + other.m };
	}
	ll eval(ll x) const {
		return k * x + m;
	}
};

struct UpperHull {
	vector<Line> hull;
	static ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b);
	}
	bool intersection_geq(Line& A, Line& B, Line& C) {
		auto left = (__int128)(B.m - A.m) * (A.k - C.k);
		auto right = (__int128)(C.m - A.m) * (A.k - B.k);
		return left >= right;
	}
	void push_back(Line& l) {
		if (sz(hull)) assert(l.k >= hull.back().k);
		if (sz(hull) && hull.back().k == l.k) {
			if (hull.back().m >= l.m) return;
			hull.pop_back();
		}
		while (sz(hull) >= 2 && intersection_geq(hull[sz(hull) - 2], hull.back(), l)) {
			hull.pop_back();
		}
		hull.push_back(l);
	}
	UpperHull() {}
	UpperHull(vector<Line>& a) {
		sort(all(a));
		for (auto l : a) {
			push_back(l);
		}
		rep(i, sz(hull) - 1) hull[i].xRight = div(hull[i + 1].m - hull[i].m, hull[i].k - hull[i + 1].k);
		if (sz(hull)) hull.back().xRight = 1e18;
	}
	static UpperHull max(const UpperHull& l, const UpperHull& r) {
		vector<Line> merged = l.hull;
		merged.insert(merged.end(), all(r.hull));
		return { merged };
	}
	static UpperHull minkowskiSum(const UpperHull& l, const UpperHull& r) {
		auto& A = l.hull;
		auto& B = r.hull;
		int i = 0, j = 0;
		vector<Line> merged;
		while (i < sz(A) && j < sz(B)) {
			merged.push_back(A[i] + B[j]);
			ll x1 = (i + 1 < sz(A) ? A[i].xRight : LLONG_MAX);
			ll x2 = (j + 1 < sz(B) ? B[j].xRight : LLONG_MAX);
			if (x1 < x2)        ++i;
			else if (x2 < x1)   ++j;
			else { ++i; ++j; }
		}
		while (i < sz(A)) merged.push_back(A[i++] + B.back());
		while (j < sz(B)) merged.push_back(A.back() + B[j++]);
		return { merged };
	}
	ll eval(ll x) {
		assert(sz(hull));
		auto it = lower_bound(all(hull), x);
		return it->eval(x);
	}
};
