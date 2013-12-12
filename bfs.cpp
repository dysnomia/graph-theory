#include <queue>
#include <utility>
#include "bfs.h"

using namespace std;

void bfs(const Graph& G, Node u, const BFSFunc& f) {
	vector<Dist> dist(G.nnodes(), INF);
	queue<Node> visit;

	/* Just visit nodes in BFS order and call f. */
	dist[u] = 0;
	visit.push(u);
	while (!visit.empty()) {
		/* Go from current node a to all of its non-marked neighbours b. */
		Node a = visit.front();	visit.pop();
		f(a, dist[a]);

		G.for_neighbours(a, [&] (Node b) {
			if (dist[b] == INF) {
				dist[b] = dist[a] + 1;
				visit.push(b);
			}
		});
	}
}

Dist distance(const Graph& G, Node u, Node v) {
	Dist dist = INF;
	bfs(G, u, [&] (Node w, Dist d) {
		if (w == v) {
			dist = d;
			/* Note: As an optimisation you can actually stop the BFS now. */
		}
	});
	return dist;
}
