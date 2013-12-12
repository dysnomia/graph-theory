#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "graph.h"

using namespace std;

Graph::Graph(istream& in) : Graph() {
	/** Read nodes. */
	string line;
	while (getline(in, line)) {
		if (line == "#") {
			break;
		}

		istringstream cur_line(line);
		Node u;
		string label;
		cur_line >> u;
		if (u <= 0) {
			ostringstream error; error << "Node '" << u << "' is invalid";
			throw runtime_error(error.str());
		}
		cur_line.ignore();
		getline(cur_line, label);

		/* Now actually add the node. */
		if (u > n) {
			labels.resize(u);
			adj.resize(u);
			n = u;
		}
		labels[u - 1] = label;
	}

	/** Read edges. */
	while (getline(in, line)) {
		istringstream cur_line(line);
		Node u, v;
		string label;
		cur_line >> u >> v;
		getline(cur_line, label); /* Edge label unused. */
		add_edge(u - 1, v - 1);
	}
}

void Graph::check(Node u) const {
	if (u < 0 || u >= n) {
		ostringstream error; error << "Node " << u << " is invalid";
		throw runtime_error(error.str());
	}
}

Node Graph::add_node(const string& label) {
	if (label != "") {
		labels.push_back(label);
	} else {
		ostringstream str; str << n+1;
		labels.push_back(str.str());
	}
	adj.push_back({});
	return n++;
}

void Graph::add_edge(Node u, Node v) {
	check(u), check(v);
	adj[u].push_back(v);
	adj[v].push_back(u);
	m++;
}

std::string Graph::label(Node u) const {
	check(u);
	return labels[u];
}

int Graph::nnodes() const {
	return n;
}

bool Graph::is_adjacent(Node u, Node v) const {
	return find(begin(adj[u]), end(adj[u]), v) != end(adj[u]);
}

int Graph::nedges() const {
	return m;
}

int Graph::degree(Node u) const {
	return int(adj[u].size());
}

void Graph::for_neighbours(Node u, const std::function<void(Node)>& f) const {
	check(u);
	for (Node v: adj[u]) {
		f(v);
	}
}

std::ostream& operator<<(std::ostream& os, const Graph& G) {
	/* Simple TGF output. */
	for (Node u = 0; u < G.nnodes(); ++u) {
		os << u + 1 << " " << G.label(u) << "\n";
	}
	os << "#\n";
	for (Node u = 0; u < G.nnodes(); ++u) {
		G.for_neighbours(u, [&] (Node v) {
			if (v >= u) {
				os << u + 1 << " " << v + 1 << "\n";
			}
		});
	}
	return os;
}

vector<vector<int>> Graph::one_step_matrix() const {
	vector<vector<int>> out(n, vector<int>(n, INF));
	for (Node u = 0; u < n; ++u) {
		out[u][u] = 0;
		for (Node v: adj[u]) {
			out[u][v] = 1;
		}
	}
	return out;
}

vector<pair<Node, Node>> Graph::edge_list() const {
	vector<pair<Node, Node>> out;
	for (Node u = 0; u < n; ++u) {
		for (Node v: adj[u]) {
			if (v > u) {
				out.push_back({u, v});
			}
		}
	}
	return out;
}

void Graph::remove_edge(Node u, Node v) {
	adj[u].erase(remove(begin(adj[u]), end(adj[u]), v), end(adj[u]));
	adj[v].erase(remove(begin(adj[v]), end(adj[v]), u), end(adj[v]));
}

Graph Graph::line_graph() const {
	Graph GL;

	/* Edges turn into nodes. */
	int line_nodes = 0;
	vector<pair<Node, Node>> nodes; /* Remeber the pair of nodes that forms a new node. */
	for (Node u = 0; u < n; ++u) {
		for (Node v: adj[u]) {
			if (v >= u) {
				ostringstream label;
				label << u << ", " << v;
				GL.add_node(label.str());
				nodes.push_back({u, v});

				/* Vertices adjacent iff they share a common endpoint. */
				for (Node w = 0; w < line_nodes; ++w) {
					if (nodes[w].first == u || nodes[w].second == u || nodes[w].first == v || nodes[w].second == v) {
						GL.add_edge(line_nodes, w);
					}
				}
				line_nodes++;
			}
		}
	}
	return GL;
}
