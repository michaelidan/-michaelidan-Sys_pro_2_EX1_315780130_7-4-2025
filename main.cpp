// michael9090124@gmail.com

#include "Graph.h"
#include "Algorithms.h"
#include <iostream> // עבור std::cout ו-std::endl

int main() {
    // בדיקה 1: הגרף המקורי עם 5 קודקודים
    std::cout << "=== Test 1: Original Graph with 5 vertices ===\n" << std::endl;
    {
        // יצירת גרף עם 5 קודקודים
        graph::Graph g(5);

        // הוספת קשתות לגרף
        g.addEdge(0, 1, 1);
        g.addEdge(0, 2, 4);
        g.addEdge(1, 2, 2);
        g.addEdge(1, 3, 5);
        g.addEdge(2, 3, 1);
        g.addEdge(2, 4, 3);
        g.addEdge(3, 4, 2);

        // הדפסת הגרף המקורי
        std::cout << "Original Graph:" << std::endl;
        g.print_graph();

        // יצירת אובייקט Algorithms עם הגרף
        graph::Algorithms alg(g);

        // BFS
        try {
            std::cout << "\nBFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph bfsTree = alg.bfs(0);
            bfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in BFS: " << e << std::endl;
        }

        // DFS
        try {
            std::cout << "\nDFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph dfsTree = alg.dfs(0);
            dfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in DFS: " << e << std::endl;
        }

        // Dijkstra
        try {
            std::cout << "\nDijkstra Tree (starting from vertex 0):" << std::endl;
            graph::Graph dijkstraTree = alg.dijkstra(0);
            dijkstraTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Dijkstra: " << e << std::endl;
        }

        // Prim
        try {
            std::cout << "\nMinimum Spanning Tree (Prim):" << std::endl;
            graph::Graph primTree = alg.prim();
            primTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Prim: " << e << std::endl;
        }

        // Kruskal
        try {
            std::cout << "\nMinimum Spanning Tree (Kruskal):" << std::endl;
            std::cout << std::flush; // נוסיף שטיפה כאן
            graph::Graph kruskalTree = alg.kruskal();
            kruskalTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Kruskal: " << e << std::endl;
        }
    }

    // בדיקה 2: גרף לא קשיר (4 קודקודים, שני חלקים נפרדים)
    std::cout << "\n=== Test 2: Disconnected Graph with 4 vertices ===\n" << std::endl;
    {
        graph::Graph g(4);

        // חלק 1: קודקודים 0 ו-1 מחוברים
        g.addEdge(0, 1, 1);

        // חלק 2: קודקודים 2 ו-3 מחוברים
        g.addEdge(2, 3, 2);

        // הדפסת הגרף
        std::cout << "Disconnected Graph:" << std::endl;
        g.print_graph();

        graph::Algorithms alg(g);

        // BFS
        try {
            std::cout << "\nBFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph bfsTree = alg.bfs(0);
            bfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in BFS: " << e << std::endl;
        }

        // DFS
        try {
            std::cout << "\nDFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph dfsTree = alg.dfs(0);
            dfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in DFS: " << e << std::endl;
        }

        // Dijkstra
        try {
            std::cout << "\nDijkstra Tree (starting from vertex 0):" << std::endl;
            graph::Graph dijkstraTree = alg.dijkstra(0);
            dijkstraTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Dijkstra: " << e << std::endl;
        }

        // Prim
        try {
            std::cout << "\nMinimum Spanning Tree (Prim):" << std::endl;
            graph::Graph primTree = alg.prim();
            primTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Prim: " << e << std::endl;
        }

        // Kruskal
        try {
            std::cout << "\nMinimum Spanning Tree (Kruskal):" << std::endl;
            std::cout << std::flush; // נוסיף שטיפה כאן
            graph::Graph kruskalTree = alg.kruskal();
            kruskalTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Kruskal: " << e << std::endl;
        }
    }

    // בדיקה 3: גרף עם משקלים שליליים (3 קודקודים)
    std::cout << "\n=== Test 3: Graph with Negative Weights (3 vertices) ===\n" << std::endl;
    {
        graph::Graph g(3);

        g.addEdge(0, 1, -1); // משקל שלילי
        g.addEdge(1, 2, 2);
        g.addEdge(0, 2, 5);

        // הדפסת הגרף
        std::cout << "Graph with Negative Weights:" << std::endl;
        g.print_graph();

        graph::Algorithms alg(g);

        // BFS
        try {
            std::cout << "\nBFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph bfsTree = alg.bfs(0);
            bfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in BFS: " << e << std::endl;
        }

        // DFS
        try {
            std::cout << "\nDFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph dfsTree = alg.dfs(0);
            dfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in DFS: " << e << std::endl;
        }

        // Dijkstra
        try {
            std::cout << "\nDijkstra Tree (starting from vertex 0):" << std::endl;
            graph::Graph dijkstraTree = alg.dijkstra(0);
            dijkstraTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Dijkstra: " << e << std::endl;
        }

        // Prim
        try {
            std::cout << "\nMinimum Spanning Tree (Prim):" << std::endl;
            graph::Graph primTree = alg.prim();
            primTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Prim: " << e << std::endl;
        }

        // Kruskal
        try {
            std::cout << "\nMinimum Spanning Tree (Kruskal):" << std::endl;
            std::cout << std::flush; // נוסיף שטיפה כאן
            graph::Graph kruskalTree = alg.kruskal();
            kruskalTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Kruskal: " << e << std::endl;
        }
    }

    // בדיקה 4: גרף ריק (0 קודקודים)
    std::cout << "\n=== Test 4: Empty Graph (0 vertices) ===\n" << std::endl;
    {
        graph::Graph g(0);

        // הדפסת הגרף
        std::cout << "Empty Graph:" << std::endl;
        g.print_graph();

        graph::Algorithms alg(g);

        // BFS
        try {
            std::cout << "\nBFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph bfsTree = alg.bfs(0);
            bfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in BFS: " << e << std::endl;
        }

        // DFS
        try {
            std::cout << "\nDFS Tree (starting from vertex 0):" << std::endl;
            graph::Graph dfsTree = alg.dfs(0);
            dfsTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in DFS: " << e << std::endl;
        }

        // Dijkstra
        try {
            std::cout << "\nDijkstra Tree (starting from vertex 0):" << std::endl;
            graph::Graph dijkstraTree = alg.dijkstra(0);
            dijkstraTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Dijkstra: " << e << std::endl;
        }

        // Prim
        try {
            std::cout << "\nMinimum Spanning Tree (Prim):" << std::endl;
            graph::Graph primTree = alg.prim();
            primTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Prim: " << e << std::endl;
        }

        // Kruskal
        try {
            std::cout << "\nMinimum Spanning Tree (Kruskal):" << std::endl;
            std::cout << std::flush; // נוסיף שטיפה כאן
            graph::Graph kruskalTree = alg.kruskal();
            kruskalTree.print_graph();
        } catch (const char* e) {
            std::cout << "Error in Kruskal: " << e << std::endl;
        }
    }

    return 0;
}
