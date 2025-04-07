// michael9090124@gmail.com

#ifndef GRAPH_H
#define GRAPH_H

namespace graph {

class Graph {
private:
    int numVertices;
    int** adjList;
    int** weights;
    int* sizes;
    bool isDirectNeighbor(int u, int v) const;

public:
    Graph(int vertices);
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    ~Graph();

    void addEdge(int src, int dest, int weight = 1);
    void removeEdge(int src, int dest);
    void print_graph();
    


    int getNumVertices() const;
    int getSize(int v) const;
    int* getAdjList(int v) const;
    int* getWeights(int v) const;
};

}

#endif
