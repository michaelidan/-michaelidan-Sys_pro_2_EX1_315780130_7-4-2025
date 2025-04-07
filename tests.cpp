// michael9090124@gmail.com

// Configure doctest to include main function in this file
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Graph.h"
#include "Algorithms.h"
#include <vector>
#include <numeric> // For std::accumulate (though not used directly here)
#include <iostream> // For potential debug printing in tests

// Use the project's namespace
using namespace graph;

// ========= Test Helper Functions ==========

// Helper function to check if an edge exists with a specific weight
bool edgeExists(const Graph& g, int u, int v, int expected_weight) {
    bool found_uv = false;
    try {
        for (int i = 0; i < g.getSize(u); ++i) {
            if (g.getAdjList(u)[i] == v && g.getWeights(u)[i] == expected_weight) {
                found_uv = true;
                break;
            }
        }
    } catch (...) { /* Ignore exceptions if u is invalid */ }

    bool found_vu = false;
     try {
        for (int i = 0; i < g.getSize(v); ++i) {
            if (g.getAdjList(v)[i] == u && g.getWeights(v)[i] == expected_weight) {
                found_vu = true;
                break;
            }
        }
     } catch (...) { /* Ignore exceptions if v is invalid */ }
    return found_uv && found_vu; // Check both directions for undirected graph
}

// Helper function to calculate total weight of a graph (for MST tests)
int getTotalWeight(const Graph& g) {
    int total_weight = 0;
    int n = g.getNumVertices();

    for (int u = 0; u < n; ++u) {
        try {
            for (int i = 0; i < g.getSize(u); ++i) {
                int v = g.getAdjList(u)[i];
                if (u < v) { // Count each edge only once for undirected graph
                    total_weight += g.getWeights(u)[i];
                }
            }
        } catch (...) { /* Ignore exceptions */ }
    }
    return total_weight;
}


// ========= Graph Class Tests ==========

TEST_CASE("Graph Class Tests") {
    SUBCASE("Constructor") {
        Graph g0(0);
        CHECK(g0.getNumVertices() == 0);
        Graph g5(5);
        CHECK(g5.getNumVertices() == 5);
        CHECK_THROWS_AS(Graph(-1), const char*); // Check throws for negative vertices
    }

    SUBCASE("Add/Remove Edges") {
        Graph g(3);
        g.addEdge(0, 1, 10);
        CHECK(g.getSize(0) == 1);
        CHECK(g.getSize(1) == 1);
        CHECK(g.getSize(2) == 0);
        CHECK(edgeExists(g, 0, 1, 10) == true);

        g.addEdge(1, 2, 5);
        CHECK(g.getSize(0) == 1);
        CHECK(g.getSize(1) == 2);
        CHECK(g.getSize(2) == 1);
        CHECK(edgeExists(g, 1, 2, 5) == true);

        // Check that duplicate adds don't change the graph
        int size_before = g.getSize(1);
        g.addEdge(1, 2, 5); // Duplicate add
        g.addEdge(2, 1, 5); // Duplicate add (reverse direction)
        CHECK(g.getSize(1) == size_before);

        g.addEdge(0, 2); // Add edge with default weight (should be 1 if implemented)
        // Assuming default weight is 1 if not provided, check accordingly
        // CHECK(edgeExists(g, 0, 2, 1) == true); // Uncomment if default weight is 1

        // Test removal
        g.removeEdge(1, 2); // Remove edge
        CHECK(edgeExists(g, 1, 2, 5) == false);
        CHECK(g.getSize(1) == 1); // Vertex 1 should only have neighbor 0 left
        CHECK(g.getSize(2) == 1); // Vertex 2 should only have neighbor 0 left (if default weight added)

        g.removeEdge(0, 1);
        CHECK(edgeExists(g, 0, 1, 10) == false);
        CHECK(g.getSize(0) == 1); // Should have neighbor 2 left (if default weight added)
        CHECK(g.getSize(1) == 0); // Vertex 1 should have no neighbors
    }

    SUBCASE("Error Handling") {
        Graph g(3);
        // Adding with invalid vertices
        CHECK_THROWS_AS(g.addEdge(-1, 0, 1), const char*);
        CHECK_THROWS_AS(g.addEdge(0, 3, 1), const char*);
        // Adding self-loop
        CHECK_THROWS_AS(g.addEdge(0, 0, 1), const char*);

        // Removing with invalid vertices
        CHECK_THROWS_AS(g.removeEdge(-1, 0), const char*);
        CHECK_THROWS_AS(g.removeEdge(0, 3), const char*);
        // Removing non-existent edge
        g.addEdge(0, 1, 10);
        CHECK_THROWS_AS(g.removeEdge(0, 2), const char*); // Edge 0-2 does not exist

        // Accessing with invalid vertex
        CHECK_THROWS_AS(g.getSize(-1), const char*);
        CHECK_THROWS_AS(g.getAdjList(3), const char*);
        CHECK_THROWS_AS(g.getWeights(3), const char*);
    }

    SUBCASE("Deep Copy") {
        Graph g_orig(3);
        g_orig.addEdge(0, 1, 10);

        // Test copy constructor
        Graph g_copy = g_orig;
        CHECK(edgeExists(g_copy, 0, 1, 10) == true);
        // Modify copy, check original is unaffected
        g_copy.addEdge(1, 2, 5);
        CHECK(edgeExists(g_orig, 1, 2, 5) == false); // Ensure edge was not added to original
        CHECK(g_orig.getSize(1) == 1);
        CHECK(g_copy.getSize(1) == 2);

        // Test assignment operator
        Graph g_assigned(2); // A completely different graph initially
        g_assigned = g_orig;
        CHECK(edgeExists(g_assigned, 0, 1, 10) == true);
        CHECK(g_assigned.getNumVertices() == 3); // Check size updated
        // Modify original, check assigned is unaffected
        g_orig.addEdge(0, 2, 1);
        CHECK(edgeExists(g_assigned, 0, 2, 1) == false);
    }
}


// ========= Algorithms Class Tests ==========

TEST_CASE("Algorithms Class Tests") {
    // Define example graphs for tests
    Graph g0(0);                      // Empty graph
    Graph g_disconnected(4);          // Disconnected graph
    Graph g_neg_weights(3);           // Graph with negative weight
    Graph g_main_example(5);          // Graph from main example (Test 1 in previous main)

    // --- Initialize Graphs ---
    // g_disconnected: 0-1, 2-3
    g_disconnected.addEdge(0, 1, 1);
    g_disconnected.addEdge(2, 3, 2);
    // g_neg_weights: 0-1 (-1), 1-2 (2), 0-2 (5)
    g_neg_weights.addEdge(0, 1, -1);
    g_neg_weights.addEdge(1, 2, 2);
    g_neg_weights.addEdge(0, 2, 5);
    // g_main_example:
    g_main_example.addEdge(0, 1, 1);
    g_main_example.addEdge(0, 2, 4);
    g_main_example.addEdge(1, 2, 2);
    g_main_example.addEdge(1, 3, 5);
    g_main_example.addEdge(2, 3, 1);
    g_main_example.addEdge(2, 4, 3);
    g_main_example.addEdge(3, 4, 2);
    // ----------------------


    SUBCASE("BFS Tests") {
        Algorithms alg(g_main_example);
        Graph bfs_tree = alg.bfs(0);
        CHECK(bfs_tree.getNumVertices() == 5);
        CHECK(edgeExists(bfs_tree, 0, 1, 1) == true);
        CHECK(edgeExists(bfs_tree, 0, 2, 4) == true);
        CHECK(edgeExists(bfs_tree, 1, 3, 5) == true); // BFS explores 1 before 2's other neighbors
        CHECK(edgeExists(bfs_tree, 2, 4, 3) == true);
        CHECK(edgeExists(bfs_tree, 1, 2, 2) == false); // Edge 1-2 not part of BFS tree from 0

        Algorithms alg0(g0);
        CHECK_THROWS_AS(alg0.bfs(0), const char*); // Invalid start vertex for empty graph
    }

    SUBCASE("DFS Tests") {
        Algorithms alg(g_main_example);
        Graph dfs_tree = alg.dfs(0);
        CHECK(dfs_tree.getNumVertices() == 5);
        // DFS path can vary, check one possibility: 0->1->2->3->4
        CHECK(edgeExists(dfs_tree, 0, 1, 1) == true);
        CHECK(edgeExists(dfs_tree, 1, 2, 2) == true);
        CHECK(edgeExists(dfs_tree, 2, 3, 1) == true);
        CHECK(edgeExists(dfs_tree, 3, 4, 2) == true);
        CHECK(edgeExists(dfs_tree, 0, 2, 4) == false); // Not part of this specific DFS path
        CHECK(edgeExists(dfs_tree, 2, 4, 3) == false); // Not part of this specific DFS path

        Algorithms alg0(g0);
        CHECK_THROWS_AS(alg0.dfs(0), const char*); // Invalid start vertex
    }

    SUBCASE("Dijkstra Tests") {
        Algorithms alg(g_main_example);
        Graph sp_tree = alg.dijkstra(0);
        CHECK(sp_tree.getNumVertices() == 5);
        // Expected shortest path tree edges from vertex 0
        CHECK(edgeExists(sp_tree, 0, 1, 1) == true); // 0 -> 1 (cost 1)
        CHECK(edgeExists(sp_tree, 1, 2, 2) == true); // 0 -> 1 -> 2 (cost 1+2=3)
        CHECK(edgeExists(sp_tree, 2, 3, 1) == true); // 0 -> 1 -> 2 -> 3 (cost 3+1=4)
        // For vertex 4: path 0->1->2->4 cost 3+3=6, path 0->1->2->3->4 cost 4+2=6
        // Depending on tie-breaking, either (2,4) or (3,4) could be chosen.
        // Let's check the total weight matches one possibility: 1+2+1+3 = 7 or 1+2+1+2 = 6
        // The previous code assumes (2,4), weight=3 => Total weight = 7. Let's check edges:
         CHECK(edgeExists(sp_tree, 2, 4, 3) == true); // If tie broken towards (2,4) edge
         // CHECK(edgeExists(sp_tree, 3, 4, 2) == true); // Or if tie broken towards (3,4) edge
         CHECK(edgeExists(sp_tree, 0, 2, 4) == false); // 0->2 direct is cost 4, path via 1 is 3
         CHECK(edgeExists(sp_tree, 1, 3, 5) == false); // path via 2 is shorter
        // CHECK(getTotalWeight(sp_tree) == 7); // Check total weight based on edges above

        Algorithms alg_neg(g_neg_weights);
        CHECK_THROWS_AS(alg_neg.dijkstra(0), const char*); // Should throw due to negative weight

        Algorithms alg0(g0);
         // Check for empty graph: should throw or return empty graph depending on implementation
         // Assuming it throws for start=0 on n=0 graph
        CHECK_THROWS_AS(alg0.dijkstra(0), const char*);
    }

    SUBCASE("Prim Tests") {
        Algorithms alg(g_main_example);
        Graph mst_prim = alg.prim();
        CHECK(mst_prim.getNumVertices() == 5);
        // MST expected weight: (0,1,1) + (1,2,2) + (2,3,1) + (3,4,2) = 6
        CHECK(getTotalWeight(mst_prim) == 6);

        Algorithms alg_disc(g_disconnected);
        Graph mst_disc = alg_disc.prim();
        CHECK(mst_disc.getNumVertices() == 4);
        // Prim from vertex 0 only finds the first component
        CHECK(getTotalWeight(mst_disc) == 1); // Only edge (0,1) weight 1
        CHECK(edgeExists(mst_disc, 0, 1, 1) == true);
        CHECK(edgeExists(mst_disc, 2, 3, 2) == false); // Should not reach the second component

        Algorithms alg_neg(g_neg_weights);
        Graph mst_neg = alg_neg.prim();
        CHECK(mst_neg.getNumVertices() == 3);
        // MST: (0,1,-1) + (1,2,2) = 1
        CHECK(getTotalWeight(mst_neg) == 1);

        Algorithms alg0(g0);
        Graph mst_empty = alg0.prim();
        CHECK(mst_empty.getNumVertices() == 0);
        CHECK(getTotalWeight(mst_empty) == 0);
    }

    SUBCASE("Kruskal Tests") {
        Algorithms alg(g_main_example);
        Graph mst_kruskal = alg.kruskal();
        CHECK(mst_kruskal.getNumVertices() == 5);
        // MST expected weight: (0,1,1) + (2,3,1) + (1,2,2) + (3,4,2) = 6
        CHECK(getTotalWeight(mst_kruskal) == 6);

        Algorithms alg_disc(g_disconnected);
        // Kruskal should throw on disconnected graph based on implementation
        CHECK_THROWS_AS(alg_disc.kruskal(), const char*);

        Algorithms alg_neg(g_neg_weights);
        Graph mst_neg = alg_neg.kruskal();
        CHECK(mst_neg.getNumVertices() == 3);
        // MST: (0,1,-1) + (1,2,2) = 1
        CHECK(getTotalWeight(mst_neg) == 1);

        Algorithms alg0(g0);
        Graph mst_empty = alg0.kruskal();
        CHECK(mst_empty.getNumVertices() == 0);
        CHECK(getTotalWeight(mst_empty) == 0);
    }
}
