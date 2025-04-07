// michael9090124@gmail.com

#include "Algorithms.h"
#include "DataStructures.h"
// #include <iostream> // Removed unnecessary comment

namespace graph {

Algorithms::Algorithms(Graph& graph) : g(graph) {}

// Performs Breadth-First Search (BFS) starting from 'start' vertex
Graph Algorithms::bfs(int start) {
    int n = g.getNumVertices();      // Number of vertices
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }
    Graph tree(n);
    bool* visited = new bool[n]();  // Tracks visited vertices
    Queue q(n);                     // Queue for managing vertices to visit
    visited[start] = true;          // Mark start vertex as visited
    q.enqueue(start);               // Add start vertex to the queue

    while (!q.isEmpty()) {          // While the queue is not empty
        int u = q.dequeue();            // Dequeue a vertex
        for (int i = 0; i < g.getSize(u); i++) {    // For each neighbor of vertex u
            int v = g.getAdjList(u)[i]; // Get neighbor v
            if (!visited[v]) {      // If neighbor v has not been visited
                visited[v] = true;  // Mark v as visited
                q.enqueue(v);       // Enqueue v
                tree.addEdge(u, v, g.getWeights(u)[i]); // Add edge to BFS tree
            }
        }
    }
    delete[] visited;               // Free memory
    return tree;                    // Return BFS tree
}

// Performs Depth-First Search (DFS) starting from 'start' vertex
Graph Algorithms::dfs(int start) {
    int n = g.getNumVertices();      // Number of vertices
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    Graph tree(n);                  // DFS tree/forest result graph
    bool* visited = new bool[n]();     // Tracks visited vertices

    // Initial call to recursive helper
    Algorithms::dfsUtil(start, visited, tree);

    // Handle other connected components
    for (int u = 0; u < n; ++u) {
        if (!visited[u]) {
            Algorithms::dfsUtil(u, visited, tree); // Start DFS from this component
        }
    }
    delete[] visited;
    return tree;                    // Return DFS tree/forest
}

// Recursive helper function for DFS
void Algorithms::dfsUtil(int u, bool* visited, Graph& tree) {
    visited[u] = true;              // Mark current vertex as visited
    for (int i = 0; i < g.getSize(u); i++) { // For each neighbor
        int v = g.getAdjList(u)[i]; // Get neighbor v
        if (!visited[v]) {          // If neighbor not visited
            tree.addEdge(u, v, g.getWeights(u)[i]); // Add edge to DFS tree
            Algorithms::dfsUtil(v, visited, tree); // Recursive call
        }
    }
}

// Dijkstra's algorithm for shortest paths
Graph Algorithms::dijkstra(int start) {
    int n = g.getNumVertices();     // Number of vertices
    if (n == 0) {                   // Handle empty graph
        return Graph(0);
    }
    if (start < 0 || start >= n) {
        throw "Invalid starting vertex!";
    }

    // Check for negative weights
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < g.getSize(u); i++) {
            if (g.getWeights(u)[i] < 0) {
                throw "Dijkstra's algorithm does'nt support negative weights!";
            }
        }
    }

    const int INF = 99999999;     // Represents infinity
    int* dist = new int[n];       // dist[i] = shortest distance from start
    int* parent = new int[n];     // parent[i] = parent in shortest path
    bool* inTree = new bool[n](); // Tracks vertices included in the SPT

    // Initialize distances and parents
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    Graph tree(n);                  // Shortest path tree result graph
    PriorityQueue pq(n);            // Priority queue for vertices
    dist[start] = 0;                // Distance to start vertex is 0
    pq.insert(start, 0);            // Insert start vertex into PQ

    while (!pq.isEmpty()) {         // While PQ is not empty
        PriorityQueue::Item item = pq.extractMin(); // Extract vertex u with minimum distance
        int u = item.vertex;
        if (inTree[u]) continue;
        inTree[u] = true;

        // If u is not the start node, add edge (parent[u], u) to the tree
        if (parent[u] != -1) {
            for (int i = 0; i < g.getSize(parent[u]); i++) {
                if (g.getAdjList(parent[u])[i] == u) {
                    tree.addEdge(parent[u], u, g.getWeights(parent[u])[i]);
                    break;
                }
            }
        }
        // Relaxation step for neighbors of u
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            int weight = g.getWeights(u)[i];
            if (!inTree[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.insert(v, dist[v]);
            }
        }
    }
    // Free memory
    delete[] dist;
    delete[] parent;
    delete[] inTree;
    return tree;                    // Return shortest path tree
}

// Prim's algorithm for Minimum Spanning Tree (MST)
Graph Algorithms::prim() {
    int n = g.getNumVertices();
    if (n == 0) {                   // Handle empty graph
        return Graph(0);
    }

    bool* inMST = new bool[n]();    // Tracks vertices included in MST
    int* key = new int[n];          // key[i] = min weight edge connecting i to MST
    int* parent = new int[n];       // parent[i] = parent in MST
    const int INF = 9999999;        // Represents infinity
    // Initialize keys and parents
    for (int i = 0; i < n; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    Graph mst(n);                   // MST result graph
    key[0] = 0;                     // Start Prim's from vertex 0

    for (int count = 0; count < n; count++) {
        // Find vertex u with minimum key
        int minKey = INF;
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break;         // Stop if graph is disconnected
        inMST[u] = true;

        // Update key and parent for neighbors of u
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            int weight = g.getWeights(u)[i];
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }

    // Build MST graph from parent array
    for (int v = 0; v < n; v++) {
        if (parent[v] != -1) {
             // Using key[v] which stores the weight of the edge chosen for v
            mst.addEdge(parent[v], v, key[v]);
        }
    }

    delete[] inMST;
    delete[] key;
    delete[] parent;

    return mst;
}

// Kruskal's algorithm for Minimum Spanning Tree (MST)
Graph Algorithms::kruskal() {
    int n = g.getNumVertices();
    if (n == 0) {                   // Handle empty graph
        return Graph(0);
    }

    // Structure to represent an edge
    struct Edge {
        int src, dest, weight;
    };

    // Collect all edges
    Edge* edges = new Edge[n * n]; // Potential over-allocation
    int edgeCount = 0;
    for (int u = 0; u < n; u++) {
        for (int i = 0; i < g.getSize(u); i++) {
            int v = g.getAdjList(u)[i];
            // Avoid duplicate edges for undirected graph
            if (u < v) {
                edges[edgeCount] = {u, v, g.getWeights(u)[i]};
                edgeCount++;
            }
        }
    }

    // Sort edges by weight (Bubble Sort)
    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = 0; j < edgeCount - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    Graph mst(n);
    // Parent array for Union-Find
    int* parent = new int[n];
    // Initialize disjoint sets
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }

    // Find operation (naive)
    auto find = [&](int u) -> int {
        if (u < 0 || u >= n) {
            throw "Invalid vertex in find!";
        }

        while (u != parent[u]) {
            u = parent[u];
            if (u < 0 || u >= n) { // Basic safety check
                throw "Invalid parent in find!";
            }
        }
        return u;
    };

    // Union operation (naive)
    auto unionSets = [&](int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        parent[rootU] = rootV; // Naive union
    };

    int edgesAdded = 0;
    // Add edges to MST
    for (int i = 0; i < edgeCount && edgesAdded < n - 1; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;
        if (find(u) != find(v)) {   // If adding edge (u, v) doesn't form a cycle
            unionSets(u, v);
            mst.addEdge(u, v, edges[i].weight);
            edgesAdded++;
        }
    }

    // Free memory
    delete[] edges;
    delete[] parent;

    // Check if MST was formed (graph connected)

    if (edgesAdded != n - 1) {
        throw "Graph is not connected - no minimum spanning tree exists";
    }

    return mst;
}

} // namespace graph
