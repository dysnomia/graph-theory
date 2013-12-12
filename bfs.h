#ifndef _BFS_H
#define _BFS_H

#include <functional>
#include "graph.h"

/**
 * BFS from u on G that calls f for each visited node v.
 * f gets the node v and the distance d(u, v).
 */
typedef std::function<void(Node, Dist)> BFSFunc;
void bfs(const Graph& G, Node u, const BFSFunc& f);

/** Computes the distance between u and v in G. */
Dist distance(const Graph& G, Node u, Node v);

#endif
