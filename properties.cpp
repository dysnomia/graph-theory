#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "graph.h"
#include "properties.h"
#include "bfs.h"

using namespace std;

/* Can we extend the current path to a Hamiltonian tour?
   pos: current node, visited: nodes already visited,
   memo: memoization array. */
static bool hamiltonian_rec(const Graph& G, Node pos, int visited,
	                        vector<vector<bool>>& memo, vector<Node>& path) {
	if (memo[pos][visited]) {
		return false;
	} else {
		memo[pos][visited] = true;

		/* Base case: try to close Hamiltonian path */
		if (visited == (1 << G.nnodes()) - 1) {
			path.push_back(pos);
			return G.is_adjacent(pos, 0);
		}

		/* Normal case: Recursive descent. */
		bool found = false;
		G.for_neighbours(pos, [&] (Node next) {
			if (!found && !(visited & (1 << next))) {
				if (hamiltonian_rec(G, next, visited | (1 << next), memo, path)) {
					found = true;
				}
			}
		});

		if (found) {
			path.push_back(pos);
		}
		return found;
	}
}

/** Simple backtracking (or DP, if you want) search for Hamiltonian tour. */
pair<bool, vector<Node>> hamiltonian_tour(const Graph& G) {
	if (G.nnodes() == 0 || G.nnodes() > 32) {
		throw runtime_error("Graph too large or trivial");
	}

	vector<Node> path;
	vector<vector<bool>> memo(G.nnodes(), vector<bool>(1 << G.nnodes(), false));
	bool found = hamiltonian_rec(G, 0, 1, memo, path);
	return {found, path};
}

/** All-pairs-shortest distance. */
vector<vector<int>> apd(const Graph& G) {
	vector<vector<int>> dist = G.one_step_matrix();
	int n = G.nnodes();

	for (Node k = 0; k < n; ++k) {
		for (Node u = 0; u < n; ++u) {
			for (Node v = 0; v < n; ++v) {
				dist[u][v] = min(dist[u][v], dist[u][k] + dist[k][v]);
			}
		}
	}
	return dist;
}

/** Eccentricity */
Dist eccentricity(const Graph& G, Node u) {
	vector<Dist> dist(G.nnodes(), INF);
	bfs(G, u, [&] (Node v, Dist d) {
		dist[v] = d;
	});
	return *max_element(begin(dist), end(dist));
}

/** Diameter calculation using BFS. */
int diam(const Graph& G) {
	int max_dist = 0;
	for (Node u = 0; u < G.nnodes(); ++u) {
		max_dist = max(max_dist, eccentricity(G, u));
	}
	return max_dist;
}

/** Girth of the graph: Remove every edge uv and then
    determine the distance between u and v. */
int girth(const Graph& G) {
	Graph G2 = G;
	auto edge_list = G.edge_list();

	int girth = INF;
	for (auto edge: edge_list) {
		Node u = edge.first, v = edge.second;
		G2.remove_edge(u, v);
		girth = min(girth, distance(G2, u, v));
		G2.add_edge(u, v);
	}

	return girth + 1;
}

/** Max. degree. */
Node maximum_degree(const Graph& G) {
	Node max_node = 0;
	int max_deg = INT_MIN;
	for (Node u = 0; u < G.nnodes(); ++u) {
		if (G.degree(u) > max_deg) {
			max_node = u;
			max_deg = G.degree(u);
		}
	}
	return max_node;
}

/* Try to color the graph using ncolours possible colours. */
bool try_colouring(const Graph& G, int ncolours, Node cur, vector<int>& colouring) {
	if (cur >= G.nnodes()) {
		return true;
	}

	/* What colours are still available for cur? */
	vector<bool> available(ncolours + 1, true);
	G.for_neighbours(cur, [&] (Node v) {
		available[colouring[v]] = false;
	});

	/* Recurse. */
	for (int colour = 1; colour <= ncolours; ++colour) {
		colouring[cur] = colour;
		if (available[colour] && try_colouring(G, ncolours, cur + 1, colouring)) {
			return true;
		}
		colouring[cur] = 0;
	}
	return false;
}

int chromatic_number(const Graph& G) {
	for (int ncolours = 1; ncolours <= G.nnodes(); ++ncolours) {
		vector<int> colouring(G.nnodes(), 0);
		if (try_colouring(G, ncolours, 0, colouring)) {
			return ncolours;
		}
	}
	return -1;
}

int chromatic_index(const Graph& G) {
	/* Possible optimisation: test only whether edge colouring
	   with \Delta(G) colours is possible and apply Vizing. */
	return chromatic_number(G.line_graph());
}
