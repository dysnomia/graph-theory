#ifndef _PROPS_H
#define _PROPS_H

#include "graph.h"

/** Tests for some simple graph properties. */

/** Simple backtracking test for Hamiltonicity. Only works for <= 32 nodes.
    (Too slow for about 18 nodes.) Needs O(n*2^n) time (NP-complete). */
std::pair<bool, std::vector<Node>> hamiltonian_tour(const Graph& G);

/** All-pairs-shortest distance in O(n^3) time. */
std::vector<std::vector<int>> apd(const Graph& G);

/* Computes the eccentricity of u in G in O(n + m) time. */
int ecc(const Graph& G, Node u);

/* Computes the diameter in O(n*(n + m))) time. */
int diam(const Graph& G);

/* Computes the girth in O(m*(m + n)) time. */
int girth(const Graph& G);

/* Computes the node with max. degree in O(n) time. */
Node maximum_degree(const Graph& G);

/* Computes the chromatic number c in O(c^n) time (output-sensitive, NP-complete). */
int chromatic_number(const Graph& G);

/* Computes the chromatic index c' in O(c'^n) time (output-sensitive, NP-complete). */
int chromatic_index(const Graph& G);

#endif
