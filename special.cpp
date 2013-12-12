#include <string>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include "special.h"

using namespace std;

/* Helper function for string to int conversion. stoi does not exist???? */
static int my_stoi(const string& s, int base) {
	int result = 0;
	for (int i = 0; i < int(s.size()); ++i) {
		result = (result * base) + (s[i] - '0');
	}
	return result;
}

Graph hypercube(int n) {
	Graph G;

	/* Consider all bit strings {0, 1}^n. */
	for (Node u = 0; u < (1 << n); ++u) {
		string label(n, '0');
		for (int bit = 0; bit < n; ++bit) {
			if (u & (1 << bit)) {
				label[bit] = '1';
			}
		}
		G.add_node(label);
	}

	/* Edges between strings with Hamming distance 1. */
	for (Node u = 0; u < (1 << n); ++u) {
		for (int bit = 0; bit < n; ++bit) {
			if (u & (1 << bit)) {
				G.add_edge(u, u & ~(1 << bit));
			}
		}
	}

	return G;
}

Graph kneser(int n, int k) {
	/* Create all subsets of [n] that have size k. */
	vector<string> labels;
	vector<int> nodes;
	string str(n, '0');
	for (int i = n - 1; i >= n - k; --i) {
		str[i] = '1';
	}
	do {
		ostringstream label;
		label << "{";
		bool first = true;
		for (int i = n - 1; i >= 0; --i) {
			if (str[i] == '1') {
				if (!first) {
					label << ", ";
				}
				label << n - i;
				first = false;
			}
		}
		label << "}";
		nodes.push_back(my_stoi(str, 2));
		labels.push_back(label.str());
	} while (next_permutation(begin(str), end(str)));

	/* Connect disjoint sets. */
	Graph G;
	for (int i = 0; i < int(nodes.size()); ++i) {
		G.add_node(labels[i]);
	}
	for (Node u = 0; u < Node(nodes.size()); ++u) {
		for (Node v = u + 1; v < Node(nodes.size()); ++v) {
			if ((nodes[u] & nodes[v]) == 0) {
				G.add_edge(u, v);
			}
		}
	}

	return G;
}

Graph empty_graph(int n) {
	Graph G;
	for (int i = 0; i < n; ++i) {
		G.add_node();
	}
	return G;
}

/** Some other graphs. */
Graph path(int n) {
	Graph G = empty_graph(n + 1);
	for (Node u = 1; u <= n; ++u) {
		G.add_edge(u, u-1);
	}
	return G;
}

Graph cycle(int n) {
	Graph G = path(n - 1);
	G.add_edge(0, n - 1);
	return G;
}

Graph complete_graph(int n) {
	Graph G = empty_graph(n);
	for (Node u = 0; u < n; ++u) {
		for (Node v = u + 1; v < n; ++v) {
			G.add_edge(u, v);
		}
	}
	return G;
}

Graph complete_bipartite(int m, int n) {
	Graph G;
	for (int i = 1; i <= m; ++i) {
		ostringstream name;
		name << "a" << i;
		G.add_node(name.str());
	}
	for (int i = 1; i <= n; ++i) {
		ostringstream name;
		name << "b" << i;
		G.add_node(name.str());
	}

	for (Node u = 0; u < m; ++u) {
		for (Node v = 0; v < n; ++v) {
			G.add_edge(u, v + m);
		}
	}
	return G;
}
