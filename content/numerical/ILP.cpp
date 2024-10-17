// Include KACTL LP solver
// ILP. Maximize
bool is_integer(double x)
{
	return abs(x - floor(x)) < 1e-9;
}

bool is_feasible(vd& x, vvd& A, vd& b, vi& integer_constraint)
{
	rep(i, sz(x))
	{
		if (integer_constraint[i] && !is_integer(x[i])) return false;
	}
	rep(i, sz(b))
	{
		double v = 0;
		rep(j, sz(x)) v += x[j] * A[i][j];
		if (v > b[i]) return false;
	}

	return true;
}

double global_lb = -inf;
void bb(int d, vvd& A, vd& b, vd& c, vi& integer_constraint)
{
	if (d > 200)
	{
		return;
	}

	LPSolver simplex(A, b, c);
	vd x(sz(c));
	double v = simplex.solve(x);
	if (v == -inf) return;
	if (v < global_lb)
	{
		return;
	}
	assert(v != inf);

	// try to obtain lower bounds
	if (is_feasible(x,A,b,integer_constraint))
	{
		global_lb = max(global_lb, v);
	}
	else
	{
		vd rounded(x);
		rep(i, sz(rounded)) rounded[i] = floor(rounded[i]);
		if (is_feasible(rounded, A, b, integer_constraint))
		{
			double lb = 0;
			rep(i, sz(rounded)) lb += rounded[i] * c[i];
			global_lb = max(global_lb, lb);
		}
		
		rounded = x;
		rep(i, sz(rounded)) rounded[i] = ceil(rounded[i]);
		if (is_feasible(rounded, A, b, integer_constraint))
		{
			double lb = 0;
			rep(i, sz(rounded)) lb += rounded[i] * c[i];
			global_lb = max(global_lb, lb);
		}
	}

	vector<tuple<double, int, int, double>> children;

	rep(v, sz(c))
	{
		if (integer_constraint[v] && !is_integer(x[v]))
		{
			vd tempx(sz(x));

			// test v <= floor(x[v])
			vd new_integrality(sz(c));
			new_integrality[v] = 1;

			b.push_back(floor(x[v]));
			A.push_back(new_integrality);
			children.emplace_back(LPSolver(A, b, c).solve(tempx), v, 1, floor(x[v]));
			b.pop_back();
			A.pop_back();

			// test v >= ceil(x[v])
			new_integrality[v] = -1;
			b.push_back(-ceil(x[v]));
			A.push_back(new_integrality);
			children.emplace_back(LPSolver(A, b, c).solve(tempx), v, -1, -ceil(x[v]));
			b.pop_back();
			A.pop_back();
		}
	}
	// branch on largest children first
	sort(all(children));
	reverse(all(children));

	repe(child, children)
	{
		double _, cc;
		int v, sign;
		tie(_, v, sign, cc) = child;
		vd new_integrality(sz(c));
		new_integrality[v] = sign;
		b.push_back(cc);
		A.push_back(new_integrality);
		bb(d + 1, A, b, c, integer_constraint);
		b.pop_back();
		A.pop_back();
	}

}
