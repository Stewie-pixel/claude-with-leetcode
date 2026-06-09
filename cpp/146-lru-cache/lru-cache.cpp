// -----------------------------------------------------------------------------
// Problem: LRU Cache
// Source : https://leetcode.com/problems/lru-cache/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// Design a data structure that follows the constraints of a Least Recently Used (LRU) cache.
//
// Implement the LRUCache class:
//
//   1. LRUCache(int capacity)
//        - Initialize the LRU cache with a positive size capacity.
//
//   2. int get(int key)
//        - Return the value of the key if it exists, otherwise return -1.
//        - When a key is accessed, it becomes the most recently used.
//
//   3. void put(int key, int value)
//        - Update the value of the key if it exists.
//        - Otherwise, insert the key-value pair.
//        - If the number of keys exceeds the capacity, evict the least recently used key.
//
// Constraints:
//   - 1 <= capacity <= 3000
//   - 0 <= key, value <= 10⁴
//   - At most 2 * 10⁵ calls will be made to get and put.
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Use a Doubly Linked List + Hash Map combination.
// 2. Each node stores (key, value), and pointers to prev / next.
// 3. The most recently used node is always at the head.
// 4. When accessing a node (via get or put):
//        - Move that node to the head.
// 5. When inserting a new node and the cache is full:
//        - Remove the tail node (least recently used).
// 6. Hash map (mp) provides O(1) access to nodes by key.
//
//  Time Complexity  : O(1) for get and put
//  Space Complexity : O(capacity)
// -----------------------------------------------------------------------------


struct Node{
    Node *next;
    Node *prev;
    int key, value;
    Node(int k, int val): key(k), value(val), next(nullptr), prev(nullptr) {}
};

class LRUCache{
private:
    int cap;                   // 1. Maximum capacity of the cache
    Node *head;                // 2. Most recently used node
    Node *tail;                // 3. Least recently used node
    map<int, Node *> mp;       // 4. Hash map: key -> corresponding node address

    // 5. Insert a new node at the head (most recently used)
    void insertAtHead(int key, int value){
        Node *newNode = new Node(key, value);
        if(head == nullptr){
            head = tail = newNode;
            mp[key] = newNode;
            return;
        }
        newNode->next = head;
        head->prev = newNode;
        mp[key] = newNode;
        head = newNode;
    }

    // 6. Delete the node at the tail (least recently used)
    void deleteAtTail(){
        Node *temp = tail;
        if(tail->prev == nullptr){
            head = tail = nullptr;
        }
        else{
            tail = tail->prev;
            tail->next = nullptr;
        }
        mp.erase(temp->key);
        delete temp;        
    }

public:
    // 7. Initialize LRU cache with given capacity
    LRUCache(int capacity): cap(capacity), head(nullptr), tail(nullptr) {}
    
    // 8. Retrieve value of a key (move accessed node to head)
    int get(int key) {
        auto it = mp.find(key);
        if(it != mp.end()){
            Node *temp = it->second;
            moveToHead(temp);
            return temp->value;
        }
        return -1; // not found
    }
    
    // 9. Insert or update a key-value pair
    void put(int key, int value) {
        auto it = mp.find(key);

        // 9.1 If key already exists → update and move to head
        if(it != mp.end()){
            Node *current = it->second;
            current->value = value;
            moveToHead(current);
            return;
        }

        // 9.2 If cache is full → remove the least recently used (tail)
        if(mp.size() == cap){
            deleteAtTail();
        }

        // 9.3 Insert new node at head
        insertAtHead(key, value);
    }

private:
    // 10. Move a node to the head (mark as most recently used)
    void moveToHead(Node *current){
        if(head == current)
            return;

        // 10.1 If node is at the tail → move tail backward
        if(tail == current){
            tail = tail->prev;
            tail->next = nullptr;
        }
        // 10.2 If node is in the middle → unlink it
        else{
            Node *temp1 = current->prev;
            Node *temp2 = current->next;
            temp1->next = temp2;
            temp2->prev = temp1;
        }

        // 10.3 Move current node to head
        current->next = head;
        head->prev = current;
        head = current;
    }
};
