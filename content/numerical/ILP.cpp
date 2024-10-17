// Use with KACTL LP solver

double global_lb = -inf;
struct node
{
	double ub;
	vd x;
	int var_index;
	int smaller;
	bool pruned = 0;
	node(int var_index, int smaller, vvd& A, vd& b, vd& c, vi& integer_constraint) : smaller(smaller), var_index(var_index)
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
	void bb(int d, vvd& A, vd& b, vd& c, vi& integer_constraint)
	{
		if (d > 250)
		{
			return;
		}
		if (ub < global_lb)
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
				children.emplace_back(new node(v, 1LL, A, b, c, integer_constraint));
				b.pop_back();
				A.pop_back();

				// test v >= ceil(x[v])
				new_integrality[v] = -1;
				b.push_back(-ceil(x[v]));
				A.push_back(new_integrality);
				children.emplace_back(new node(v, 0LL, A, b, c, integer_constraint));
				b.pop_back();
				A.pop_back();
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
			child->bb(d + 1, A, b, c, integer_constraint);
			b.pop_back();
			A.pop_back();
		}
	}
};

