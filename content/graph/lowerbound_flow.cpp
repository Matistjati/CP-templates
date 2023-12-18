struct edge
{
	int from, to, lo, hi;
};
typedef PushRelabel flowAlgorithm;
// Returns null if no feasible flow exists with given lower bounds
// To find min flow satisfying constraints, binary search for smallest capacity giving feasible solution
flowAlgorithm* lowerbound_flow(vector<edge>& edges, int n, int source, int sink, int capacity=inf)
{
	flowAlgorithm* flow = new flowAlgorithm(n + 2);
	vi outflow(n), inflow(n);
	repe(e, edges)
	{
		outflow[e.from] += e.lo;
		inflow[e.to] += e.lo;
		flow->addEdge(e.from, e.to, e.hi - e.lo);
	}
	rep(i, n)
	{
		flow->addEdge(n, i, inflow[i]);
		flow->addEdge(i, n+1, outflow[i]);
	}
	flow->addEdge(sink, source, capacity);

	return flow->calc(n,n+1)==accumulate(all(outflow),0) ? flow : nullptr;
}

