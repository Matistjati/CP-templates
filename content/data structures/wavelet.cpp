struct Node
{
	Node* l = 0, * r = 0;
	int lo, hi;
	vi mapleft;

	// Numbers in a are in [lo, hi)
	Node(vi& a, int lo, int hi) : lo(lo), hi(hi), mapleft(1, 0)
	{
		if (lo + 1 == hi) return;
		int mid = (lo + hi) / 2;
		vi L, R;
		repe(v, a)
		{
			mapleft.push_back(mapleft.back());
			if (v < mid) L.push_back(v), mapleft.back()++;
			else R.push_back(v);
		}

		l = new Node(L, lo, mid); r = new Node(R, mid, hi);
	}

	// Kth (0-indexed) largest number in [l, r) 
	int quantile(int k, int l, int r)
	{
		if (lo + 1 == hi) return lo;
		int c = mapleft[r] - mapleft[l];
		if (k < c) return this->l->quantile(k, mapleft[l], mapleft[r]);
		return this->r->quantile(k - c, l - mapleft[l], r - mapleft[r]);
	}
};