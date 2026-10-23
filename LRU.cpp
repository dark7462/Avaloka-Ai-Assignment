#include <unordered_map>
#include <iostream>
#include <chrono>
using namespace std;

class Node {
public:
    int val, key;
    Node* prev;
    Node* next;
    Node(int key, int val) {
        this->key = key;
        this->val = val;
        this->prev = NULL;
        this->next = NULL;
    }
};

class LRUCache{
private:
    unordered_map<int, Node*> cache;
    int cap;
    Node* LRU; // head
    Node* MRU; // tail
    /*
        Insert at tail so that is most recently used
        And delete at head that’s least recently used
    */

public:
    LRUCache(int cap) {
        this->cap = cap;
        LRU = MRU = NULL;
    }

    void insert(Node *newNode) {
        if(MRU == NULL) {
            LRU = MRU = newNode;
        } else {
            newNode->prev = MRU;
            newNode->next = NULL;
            MRU->next = newNode;
            MRU = newNode;
        }
    }
    void remove(Node *node) {
        if(node->prev != NULL) {
            node->prev->next = node->next; // if not head
        } else {
            LRU = node->next;
        }

        if(node->next != NULL) {
            node->next->prev = node->prev; // if not tail
        } else{
            MRU = node->prev;
        }
    }

    int get(int key) {
        // if present then value else -1
        if(cache.find(key) != cache.end()) {
            remove(cache[key]);
            insert(cache[key]);
            return cache[key]->val;
        } else {
            return -1;
        }
    }
    void put(int key, int value) {
        // edge case
        if(cap <= 0){
            cout << "Cache can't be stored 0 capacity";
            return;
        }
        // update
        if(cache.find(key) != cache.end()) {
            cache[key]->val = value;
            remove(cache[key]);
            insert(cache[key]);
            return;
        }

        // insert
        if (cache.size() >= cap) { // limit reached
            Node* dummy = LRU; // need to keep the variable LRU (node LRU we'll shift later)
            cache.erase(dummy->key); // from map
            remove(dummy); // from list
            delete dummy; // from memory too 
        }

        // if not updated and size is limits
        Node* newnode = new Node(key, value);
        insert(newnode); // in list
        cache[key] = newnode; // in map
    }
    // to free up all the memory
    ~LRUCache() {
        Node* curr = LRU;
        while (curr != nullptr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

int main() {
    const int CAPACITY = 1000;
    const int OPERATIONS = 1000000;

    LRUCache cache(CAPACITY);

    auto start = chrono::steady_clock::now();

    for (int i = 0; i < OPERATIONS; i++) {
        cache.put(i, i * 10);
        cache.get(i);

        // Mostly cache misses
        if (i >= CAPACITY)
            cache.get(i - CAPACITY);
    }

    auto end = chrono::steady_clock::now();

    cout << "Operations: " << OPERATIONS << '\n';
    cout << "Capacity: " << CAPACITY << '\n';
    cout << "Execution time: "
         << chrono::duration<double, milli>(end - start).count()
         << " ms\n";

    cout << "Last key: " << cache.get(OPERATIONS - 1) << '\n';
    cout << "Evicted key: " << cache.get(0) << '\n';

    // LRUCache cache1(0);

    // cache1.put(1, 100);

    // cout << cache1.get(1) << endl;

}