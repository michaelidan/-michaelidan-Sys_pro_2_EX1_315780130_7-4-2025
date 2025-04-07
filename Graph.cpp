// michael9090124@gmail.com

#include "Graph.h"
#include <iostream> 

namespace graph {

    // Check for direct neighbor (u -> v)
    bool Graph::isDirectNeighbor(int u, int v) const {
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            return false; // Invalid index
        }
        for (int i = 0; i < sizes[u]; i++) {
            if (adjList[u][i] == v) return true;   // Found v in u's neighbor list
        }
        return false; // v not found in u's list
    }

    Graph::Graph(int vertices) : numVertices(vertices) { // Constructor
        if (vertices < 0) { // Check for negative number of vertices
            throw "Number of vertices cannot be negative!";
        }
        adjList = new int *[numVertices];   // Adjacency lists
        weights = new int *[numVertices];   // Corresponding weights
        sizes = new int[numVertices];    // Size of each adjacency list

        // Initialize lists and sizes
        for (int i = 0; i < numVertices; i++) {
            adjList[i] = nullptr;
            weights[i] = nullptr;
            sizes[i] = 0;
        }
    }

    Graph::Graph(const Graph &other) : numVertices(other.numVertices) { // Copy constructor
        adjList = new int *[numVertices];
        weights = new int *[numVertices];
        sizes = new int[numVertices];

        for (int i = 0; i < numVertices; i++) {
            sizes[i] = other.sizes[i];
            if (sizes[i] > 0) { // If adjacency list is not empty, allocate memory
                adjList[i] = new int[sizes[i]];
                weights[i] = new int[sizes[i]];
                for (int j = 0; j < sizes[i]; j++) {
                    adjList[i][j] = other.adjList[i][j];
                    weights[i][j] = other.weights[i][j];
                }
            }
            else {
                adjList[i] = nullptr;
                weights[i] = nullptr;
            }
        }
    }

    Graph& Graph::operator=(const Graph& other) {    // Assignment operator
        if (this == &other) {   // Check for self-assignment
            return *this;   // Return current object if self-assigning
        }
        // Delete existing resources
        for (int i = 0; i < numVertices; i++) {
            if (sizes[i] > 0) {
                delete[] adjList[i];
                delete[] weights[i];
            }
        }
        delete[] adjList;
        delete[] weights;
        delete[] sizes;

        // Copy data from other graph
        numVertices = other.numVertices;
        adjList = new int *[numVertices];
        weights = new int *[numVertices];
        sizes = new int[numVertices];

        for (int i = 0; i < numVertices; i++) {
            sizes[i] = other.sizes[i];
            if (sizes[i] > 0) {
                adjList[i] = new int[sizes[i]];
                weights[i] = new int[sizes[i]];
                for (int j = 0; j < sizes[i]; j++) {
                    adjList[i][j] = other.adjList[i][j];
                    weights[i][j] = other.weights[i][j];
                }
            }
            else {
                adjList[i] = nullptr;
                weights[i] = nullptr;
            }
        }
        return *this;
    }

    Graph::~Graph() { // Destructor
        for (int i = 0; i < numVertices; i++) {
            if (sizes[i] > 0) { // Check if the list for vertex i has allocated memory
                delete[] adjList[i];
                delete[] weights[i];
            }
        }
        delete[] adjList;
        delete[] weights;
        delete[] sizes;
    }

    // Add an edge to the graph
    void Graph::addEdge(int src, int dest, int weight) {
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
             throw "Invalid vertex!";
        }
        if (src == dest) {
            throw "Self-loops are not allowed!";
        }

        if (isDirectNeighbor(src, dest)) return; // Check if edge already exists

        // Reallocate and copy for src -> dest
        int newSzSrc = sizes[src] + 1;
        int* newAdjSrc= new int[newSzSrc];
        int* newWeSrc = new int[newSzSrc];
        for (int i = 0; i < sizes[src]; i++) {
            newAdjSrc[i] = adjList[src][i];
            newWeSrc[i] = weights[src][i];
        }
        newAdjSrc[sizes[src]] = dest; // Add the new neighbor
        newWeSrc[sizes[src]] = weight;

        if (sizes[src] > 0) { // Free old arrays if they existed
            delete[] adjList[src];
            delete[] weights[src];
        }
        adjList[src] = newAdjSrc;   // Update pointers
        weights[src] = newWeSrc;
        sizes[src] = newSzSrc;     // Update size

        // Since graph is assumed undirected, add the edge in the other direction too
        // Note: Recursive call might lead to stack overflow for large graphs/sequences.
        // An iterative approach or checking existence before calling addEdge might be safer.
        if (!isDirectNeighbor(dest, src)) { // Avoid infinite recursion if somehow called improperly
             addEdge(dest, src, weight); // Add edge dest -> src
        }
    }

    void Graph::removeEdge(int src, int dest) { // Remove an edge from the graph
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
            throw "Invalid vertex!";
        }
        int index_src = -1; // Index of dest in src's list
        for (int i = 0; i < sizes[src]; i++) {
            if (adjList[src][i] == dest) {
                index_src = i;
                break;
            }
        }
        if (index_src == -1) { // If edge src->dest doesn't exist
            throw "Edge does not exist!";
        }

        // Remove dest from src's list
        { // Scope for temporary variables for src->dest removal
            int originalSize = sizes[src];
            int newSize = originalSize - 1;
            int* newAdj = nullptr;
            int* newWeights = nullptr;

            if (newSize > 0) { // Create smaller arrays if the list won't be empty
                newAdj = new int[newSize];
                newWeights = new int[newSize];
                // Copy elements, skipping the one at index_src
                for (int i = 0, j = 0; i < originalSize; i++) {
                    if (i != index_src) {
                        newAdj[j] = adjList[src][i];
                        newWeights[j] = weights[src][i];
                        j++;
                    }
                }
            }
            // Free old arrays for src
            delete[] adjList[src];
            delete[] weights[src];

            // Update src's pointers and size
            adjList[src] = newAdj;
            weights[src] = newWeights;
            sizes[src] = newSize;
        } // End of temporary scope

        // Check if the reverse edge (dest->src) still exists and remove it recursively if needed
        // Note: Potential for stack overflow with recursive calls for large graphs.
        if (isDirectNeighbor(dest, src)) {
            removeEdge(dest, src); // Remove edge dest -> src
        }
    }

    // Print the graph representation
    void Graph::print_graph() {
        for (int i = 0; i < numVertices; i++) {
            std::cout << "Vertex " << i << ": {";
            for (int j = 0; j < sizes[i]; j++) {
                std::cout << adjList[i][j] << " (" << weights[i][j] << ")";
                if (j < sizes[i] - 1) {
                    std::cout << ",  ";
                }
            }
            std::cout << "}" << std::endl;
        }
    }

    // Getters
    int Graph::getNumVertices() const {
        return numVertices;
    }

    int Graph::getSize(int v) const { // Number of neighbors for vertex v
        if (v < 0 || v >= numVertices) {
            throw "Invalid vertex!";
        }
        return sizes[v];
    }

    int* Graph::getAdjList(int v) const { // Adjacency list for vertex v
        if (v < 0 || v >= numVertices) {
            throw "Invalid vertex!";
        }
        return adjList[v];
    }

    int* Graph::getWeights(int v) const { // Weights list for vertex v
        if (v < 0 || v >= numVertices) {
            throw "Invalid vertex!";
        }
        return weights[v];
    }
} // namespace graph
