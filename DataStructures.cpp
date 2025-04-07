// michael9090124@gmail.com

#include "DataStructures.h"
#include <iostream> // Keep include, remove comment

namespace graph {

// --- Queue ---

Queue::Queue(int cap) {
    capacity = cap;         // Initial capacity
    array = new int[cap];   // Dynamic array allocation
    front = 0;              // Front starts at 0
    rear = -1;              // Rear starts before front
    size = 0;               // Queue is initially empty
}

Queue::~Queue() {
    delete[] array;         // Free array memory
}

void Queue::enqueue(int x) {
    if (size == capacity) { // If queue is full
        int* newArray = new int[capacity * 2]; // Double the capacity
        for (int i = 0; i < size; i++) {
             // Copy to new array (handling wrap-around)
            newArray[i] = array[(front + i) % capacity];
        }
        delete[] array;         // Free old array
        array = newArray;       // Update pointer
        front = 0;              // Reset front
        rear = size - 1;        // Update rear
        capacity *= 2;          // Update capacity
    }
    rear = (rear + 1) % capacity; // Update rear circularly
    array[rear] = x;            // Add the element
    size++;                     // Increment size
}

int Queue::dequeue() {
    if (isEmpty()) {
        throw "Queue is empty!"; // Translated exception
    }
    int item = array[front];    // Get item from front
    front = (front + 1) % capacity; // Update front circularly
    size--;                     // Decrement size
    return item;                // Return the removed item
}

bool Queue::isEmpty() {
    return size == 0;           // Returns true if the queue is empty
}

// --- PriorityQueue ---
// Note: This is an inefficient implementation (O(N) extractMin).
// A heap-based implementation is standard for efficiency.

PriorityQueue::PriorityQueue(int cap) {
    capacity = cap;         // Initial capacity
    array = new Item[cap];  // Dynamic array of Items
    size = 0;               // Queue is initially empty
}

PriorityQueue::~PriorityQueue() {
    delete[] array;         // Free array memory
}

void PriorityQueue::insert(int v, int p) {
    if (size == capacity) { // If queue is full
        Item* newArray = new Item[capacity * 2]; // Double the capacity
        for (int i = 0; i < size; i++) {
            newArray[i] = array[i]; // Copy to new array
        }
        delete[] array;         // Free old array
        array = newArray;       // Update pointer
        capacity *= 2;          // Update capacity
    }
    array[size].vertex = v;     // Add vertex
    array[size].priority = p;   // Add priority
    size++;                     // Increment size
}

PriorityQueue::Item PriorityQueue::extractMin() {
    if (isEmpty()) {
        throw "Priority queue is empty!"; // Translated exception
    }
    int minIndex = 0;           // Index of minimum item, start at 0
    for (int i = 1; i < size; i++) { // Linear scan for minimum
        if (array[i].priority < array[minIndex].priority) {
            minIndex = i;
        }
    }
    Item minItem = array[minIndex]; // Store the minimum item
    array[minIndex] = array[size - 1]; // Move last element to the vacated spot
    size--;                     // Decrement size
    return minItem;             // Return the minimum item
}

bool PriorityQueue::isEmpty() {
    return size == 0;           // Returns true if the queue is empty
}

// --- UnionFind ---
// Implements Disjoint Set Union (DSU) with Path Compression and Union by Rank.

UnionFind::UnionFind(int n) {
    size = n;               // Number of elements
    parent = new int[n];    // Parent array allocation
    rank = new int[n];      // Rank array allocation
    makeSet();              // Initialize sets
}

UnionFind::~UnionFind() {
    delete[] parent;        // Free parent array
    delete[] rank;          // Free rank array
}

void UnionFind::makeSet() {
    for (int i = 0; i < size; i++) {
        parent[i] = i;      // Each element is initially its own root
        rank[i] = 0;        // Initial rank is 0
    }
}

int UnionFind::find(int x) {
    // Find the root of the set containing x, with path compression
    if (parent[x] != x) {   // If x is not the root
        parent[x] = find(parent[x]); // Path compression: connect directly to root
    }
    return parent[x];       // Return the root
}

void UnionFind::unionSets(int x, int y) {
    int rootX = find(x);    // Find root of x
    int rootY = find(y);    // Find root of y
    if (rootX == rootY) {   // If already in the same set
        return;
    }
    // Union by rank: attach shorter tree under taller tree
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        // Ranks are equal: attach y to x and increment rank of x
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

} // namespace graph
