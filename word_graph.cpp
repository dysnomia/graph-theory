#include <iostream>
#include <string>
#include <utility>
#include <cassert>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "graph.h"
#include "bfs.h"
#include "properties.h"

using namespace std;

/** word_graph.cpp: Program for generating and analysing a word graph. */

/** Computes the Hamming distance of s1 and s2. */
int hamming(const string& s1, const string& s2) {
	assert(s1.size() == s2.size());
	int diff = 0;
	for (size_t i = 0; i < s1.size(); ++i) {
		if (s1[i] != s2[i]) {
			diff++;
		}
	}
	return diff;
}

/**
 * Reads the word graph from the stream 'in'.
 *   nodes: words of length word_length.
 *   edges: between words of Hamming distance 1.
 */
Graph read_graph(istream& in, int word_length) {
	Graph G;

	/* Definition of a word: first space-delimited string on a line starting with
	   an alphabetic character. It is normalised to lower case. */
	string line, word;
	while (getline(in, line)) {
		if (line.length() > 0 && isalpha(line[0])) {
			istringstream(line) >> word;
			transform(begin(word), end(word), begin(word), ::tolower);

			if (int(word.length()) == word_length) {
				Node u = G.add_node(word);

				for (Node v = 0; v < G.nnodes() - 1; ++v) {
					if (hamming(word, G.label(v)) == 1) {
						G.add_edge(u, v);
					}
				}
			}
		}
	}
	return G;
}

/** Determines the order and diameter of the components of G in O(n(n + m)) time. */
struct Summary {
	size_t order;
	Dist diam;
	string repr;
};
vector<Summary> analyse_connectivity(const Graph& G) {
	/* Compute component and eccentricity (in component) of all nodes. */
	int ncomps = 0;
	int n = G.nnodes();
	vector<int> component(n, -1), eccentricity(n, 0);
	for (Node u = 0; u < n; ++u) {
		int& ecc = eccentricity[u];
		int cur_comp = (component[u] != -1) ? component[u] : (ncomps++);

		bfs(G, u, [&] (Node v, Dist d) {
			component[v] = cur_comp;
			ecc = max(ecc, d);
		});
	}

	/* Create summary for each component: order, diameter and representative node. */
	vector<Summary> summaries(ncomps, {0, 0, ""});
	for (Node u = 0; u < n; ++u) {
		auto& s = summaries[component[u]];
		s.order++;
		s.diam = max(s.diam, eccentricity[u]);
		s.repr = G.label(u);
	}
	return summaries;
}

int main(int argc, char** argv) {
	/* Read and analyse the word graph. */
	cout << "Usage: " << argv[0] << " [word length] < dict.txt\n";
	int word_length = 3;
	if (argc >= 2) {
		sscanf(argv[1], "%d", &word_length);
	}
	Graph G = read_graph(cin, word_length);
	auto summaries = analyse_connectivity(G);

	/* Print components. */
	cout << "Word graph with " << G.nnodes() << " words of length " << word_length << "\n";
	cout << " - " << G.nedges() << " edges\n";
	Node max_node = maximum_degree(G);
	cout << " - Node with max. degree " << G.degree(max_node) << ": " << G.label(max_node) << "\n";

	cout << summaries.size() << " components:\n";
	sort(begin(summaries), end(summaries), [] (const Summary& s1, const Summary& s2) {
		return s1.order > s2.order;
	});
	for (Summary s: summaries) {
		cout << " - Order " << setw(3) << s.order << " with diameter " << setw(2) << s.diam << " (representative: " << s.repr << ")\n";
	}
	return 0;
}
