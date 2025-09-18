struct Node
{
	Node* lchild;
	Node* rchild;
	int l, r;
	int sum;
	int lazy;

	Node(int l, int r) : lchild(nullptr), rchild(nullptr), l(l), r(r), sum(0), lazy(0) {}

	void push()
	{
		int mid = (l + r) / 2;
		if (!lchild)
		{
			lchild = new Node(l, mid);
			rchild = new Node(mid + 1, r);
		}
		lchild->sum = lchild->sum + lazy * (mid - l + 1);
		lchild->lazy = lchild->lazy + lazy;

		rchild->sum = rchild->sum + lazy * (r - (mid + 1) + 1);
		rchild->lazy = rchild->lazy + lazy;
		lazy = 0;
	}

	void add(int ql, int qr, int v)
	{
		if (l > qr || r < ql) return;
		if (l >= ql && r <= qr)
		{
			sum = sum + v * (r - l + 1);
			lazy = lazy + v;
		}
		else
		{
			push();
			lchild->add(ql, qr, v);
			rchild->add(ql, qr, v);
			sum = rchild->sum + lchild->sum;
		}
	}

	int query(int ql, int qr)
	{
		if (l > qr || r < ql) return 0;
		if (l >= ql && r <= qr) return sum;
		else
		{
			push();
			return lchild->query(ql, qr) + rchild->query(ql, qr);
		}
	}
};

