// michael9090124@gmail.com

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

namespace graph {

class Queue {
private:
    int* array;
    int capacity;
    int front;
    int rear;
    int size;

public:
    Queue(int cap);
    ~Queue();
    void enqueue(int x);
    int dequeue();
    bool isEmpty();
};

class PriorityQueue {
public:
    struct Item {
        int vertex;
        int priority;
    };

private:
    Item* array;
    int capacity;
    int size;

public:
    PriorityQueue(int cap);
    ~PriorityQueue();
    void insert(int v, int p);
    Item extractMin();
    bool isEmpty();
};

class UnionFind {
private:
    int* parent;
    int* rank;
    int size;

public:
    UnionFind(int n);
    ~UnionFind();
    void makeSet();
    int find(int x);
    void unionSets(int x, int y);
};

}

#endif
