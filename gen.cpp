#include <iostream>
#include <sstream>
#include "graph.h"
#include "special.h"

using namespace std;

/** gen.cpp: Program for generating special graphs in TGF format. */
int main(int argc, char** argv) {
	if (argc <= 1) {
		cerr << "Usage: " << argv[0] << " <name of graph>\n";
		cerr << "Graphs:\n";
		cerr << "  Qn:    hypercube\n";
		cerr << "  KGn,k: Kneser graph\n";
		cerr << "  Km,n:  complete bipartite graph\n";
		cerr << "  Kn:    complete graph\n";
		cerr << "  En:    empty graph\n";
		cerr << "  Pn:    path\n";
		cerr << "  Cn:    cycle\n";
		return 1;
	}

	istringstream in(argv[1]);
	char type, tmp;
	in >> type;
	if (type == 'Q') { /* hypercube */
		int size;
		in >> size;
		cout << hypercube(size);
	} else if (type == 'K') {
		if (in.peek() == 'G') { /* kneser */
			in.get();
			int n, k;
			in >> n >> tmp >> k;
			cout << kneser(n, k);
		} else { /* complete or complete bipartite graph. */
			int n, m;
			in >> n;
			if (in >> tmp >> m) {
				cout << complete_bipartite(n, m);
			} else {
				cout << complete_graph(n);
			}
		}
	} else if (type == 'E') { /* empty graph */
		int n; in >> n;
		cout << empty_graph(n);
	} else if (type == 'P') { /* path */
		int n; in >> n;
		cout << path(n);
	} else if (type == 'C') { /* cycle */
		int n; in >> n;
		cout << cycle(n);
	} else {
		cerr << "Graph type '" << type << "' unknown\n";
		return 1;
	}
	return 0;
}
