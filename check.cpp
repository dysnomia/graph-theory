#include <iostream>
#include <sstream>
#include "graph.h"
#include "properties.h"

using namespace std;

/** Format integer value that can be infinite nicely. */
string inf(int n) {
	stringstream out;
	if (n == INF) {
		out << "infinity";
	} else {
		out << n;
	}
	return out.str();
}

/** check.cpp: Program for determining some properties of a graph in TGF format. */
int main(int argc, char** argv) {
	cout << "Usage: " << argv[0] << " < graph.tgf\n";

	/* Read graph. */
	Graph G(cin);

	/* Order and size. */
	cout << "n = " << G.nnodes() << "\n";
	cout << "m = " << G.nedges() << "\n";

	/* Diameter */
	cout << "diam = " << inf(diam(G)) << "\n";

	/* Girth */
	cout << "girth = " << inf(girth(G)) << "\n";

	/* Hamiltonicity. */
	cout << "Hamiltonian: ";
	if (G.nnodes() <= 16) {
		bool is_hamiltonian;
		vector<Node> tour;
		tie(is_hamiltonian, tour) = hamiltonian_tour(G);
		cout << (is_hamiltonian ? "Yes" : "No");
		if (is_hamiltonian) {
			cout << " (";
			for (Node u: tour) {
				cout << G.label(u);
				if (u != 0) {
					cout << ", ";
				}
			}
			cout << ")";
		}
	} else {
		cout << "not checked";
	}
	cout << "\n";

	/* Chromatic number. */
	cout << "Chromatic number: ";
	if (G.nnodes() <= 16) {
		cout << chromatic_number(G);
	} else {
		cout << "not checked";
	}
	cout << "\n";

	/* Chromatic index. */
	cout << "Chromatic index: ";
	if (G.nnodes() <= 16) {
		cout << chromatic_index(G);
	} else {
		cout << "not checked";
	}
	cout << "\n";

	return 0;
}
