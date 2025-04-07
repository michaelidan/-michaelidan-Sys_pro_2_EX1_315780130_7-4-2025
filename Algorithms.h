// michael9090124@gmail.com

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"

namespace graph {

class Algorithms {
private:
    Graph& g;
    void dfsUtil(int u, bool* visited, Graph& tree);

public:
    Algorithms(Graph& graph); 

    Graph bfs(int source); 
    Graph dfs(int source); 
    Graph dijkstra(int start);
    Graph prim(); 
    Graph kruskal(); 
};

} 

#endif
