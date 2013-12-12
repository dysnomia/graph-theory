#ifndef _SPECIAL_H
#define _SPECIAL_H

#include "graph.h"

/** special.h: Generators for special graphs. */

/** Hypercube of dimension n. */
Graph hypercube(int n);

/** Kneser graph with parameters n and k. */
Graph kneser(int n, int k);

/** Empty graph of order n. */
Graph empty_graph(int n);

/** Path of length n. */
Graph path(int n);

/** Cycle of length n. */
Graph cycle(int n);

/** Complete graph of order n. */
Graph complete_graph(int n);

/** Complete bipartite graph with partite sets of size m and n. */
Graph complete_bipartite(int m, int n);

#endif
