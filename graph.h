#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <string>
#include <functional>
#include <climits>

/* Large value representing infinity such that INF + INF does not overflow. */
const int INF = INT_MAX / 4;

/**
 * Undirected multigraph with adjacency list representation.
 * You may assume that nodes are given ids from 0 to nnodes() - 1.
 */
typedef int Node;
typedef int Dist;
class Graph {
private:
	std::vector<std::string> labels;    /* Node labels. */
	std::vector<std::vector<Node>> adj; /* Adjacency list. */
	int n, m;                           /* #nodes & #edges */

	/** Checks whether the node id is valid. */
	void check(Node u) const;

public:
	/* Default, copy & move construction. */
	Graph() : labels(), adj(), n(0), m(0) {};
	Graph(const Graph&) = default;
	Graph(Graph&&) = default;

	/* Generates an undirected graph from a TGF file (little error checking!). */
	Graph(std::istream& in);

	/* Adds nodes or edges in O(1) amortised time. */
	Node add_node(const std::string& label = "");
	void add_edge(Node u, Node v);

	/* Removes an edge in O(deg(u) + deg(v)) time. */
	void remove_edge(Node u, Node v);

	/* Tests whether u is adjacent to v in O(deg(u)) time. */
	bool is_adjacent(Node u, Node v) const;

	/* Getters. */
	std::string label(Node u) const;
	int nnodes() const;
	int nedges() const;

	/* Computes the degree of node u in O(1) time. */
	int degree(Node u) const;

	/* Calls f(v) for all neighbours v of u. */
	void for_neighbours(Node u, const std::function<void(Node)>& f) const;

	/* Generates the distance matrix in one step, i.e. 0 from u to u,
	   1 if u and v are adjacent, INF otherwise in O(n^2) time. */
	std::vector<std::vector<int>> one_step_matrix() const;

	/* Generates the edge list of the graph in O(n + m) time. */
	std::vector<std::pair<Node, Node>> edge_list() const;

	/* Generates the corresponding line graph in O(n + m^2) time. */
	Graph line_graph() const;
};

/** Prints a graph in TGF format. */
std::ostream& operator<<(std::ostream& os, const Graph& G);

#endif
