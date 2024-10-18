// Max ILP solver, use with KACTL LP solver

// ILP. Maximize
bool is_integer(double x)
{
	return abs(x - floor(x)) < eps;
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

// notes: don't forget to set time_limit, depth, and integer_constraint
const int time_limit = 950;
auto start = chrono::high_resolution_clock::now();
double global_lb = -inf;
vvd A;
vd b, c;
vi integer_constraint;
struct node
{
	double ub;
	vd x;
	int var_index;
	int smaller;
	bool pruned = 0;
	node(int var_index, int smaller) : smaller(smaller), var_index(var_index)
	{
		x.resize(sz(c));
		ub = LPSolver(A, b, c).solve(x);
		if (ub == -inf) pruned = 1;
		if (ub < global_lb)
		{
			pruned = 1;
		}

		auto check_sol = [&](vd& x)
		{
			if (!is_feasible(x, A, b, integer_constraint)) return false;
			double value = 0;
			rep(i, sz(x)) value += x[i] * c[i];
			global_lb = max(global_lb, value);
			return true;
		};

		// try to obtain lower bounds
		if (check_sol(x))
		{
			// if simplex gave us integers, no point in branching from this node
			pruned = 1;
		}
		else
		{
			// try to round for bounds
			vd rounded(x);
			rep(i, sz(rounded)) rounded[i] = floor(rounded[i]);
			check_sol(rounded);

			rounded = x;
			rep(i, sz(rounded)) rounded[i] = ceil(rounded[i]);
			check_sol(rounded);

			rounded = x;
			rep(i, sz(rounded)) rounded[i] = round(rounded[i]);
			check_sol(rounded);
		}
	}
	void branch(int d)
	{
		if (d > 75)
		{
			return;
		}
		if (ub < global_lb)
		{
			return;
		}
		if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() > time_limit)
		{
			return;
		}

		vector<node*> children;

		rep(v, sz(c))
		{
			if (integer_constraint[v] && !is_integer(x[v]))
			{
				vd new_integrality(sz(c));
				// test v <= floor(x[v])
				new_integrality[v] = 1;
				b.push_back(floor(x[v]));
				A.push_back(new_integrality);
				children.emplace_back(new node(v, 1LL));
				b.pop_back();
				A.pop_back();

				// test v >= ceil(x[v])
				new_integrality[v] = -1;
				b.push_back(-ceil(x[v]));
				A.push_back(new_integrality);
				children.emplace_back(new node(v, 0LL));
				b.pop_back();
				A.pop_back();
				break;
			}
		}
		// branch on largest children first
		sort(all(children), [](node* a, node* b)
			{
				return a->ub > b->ub;
			});

		repe(child, children)
		{
			if (child->pruned) continue;
			int idx = child->var_index;
			vd new_integrality(sz(c));
			new_integrality[idx] = child->smaller ? 1 : -1;
			b.push_back(child->smaller ? floor(x[idx]) : -ceil(x[idx]));
			A.push_back(new_integrality);
			child->branch(d + 1);
			b.pop_back();
			A.pop_back();
		}
	}
};
