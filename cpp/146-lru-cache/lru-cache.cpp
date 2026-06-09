struct Node{
    Node *next;
    Node *prev;
    int key, value;
    Node(int k, int val): key(k), value(val), next(nullptr), prev(nullptr) {}
};

class LRUCache{
private:
    int cap;
    Node *head;
    Node *tail;
    map<int, Node *> mp;

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

    LRUCache(int capacity): cap(capacity), head(nullptr), tail(nullptr) {}

    int get(int key) {
        auto it = mp.find(key);
        if(it != mp.end()){
            Node *temp = it->second;
            moveToHead(temp);
            return temp->value;
        }
        return -1;
    }

    void put(int key, int value) {
        auto it = mp.find(key);

        if(it != mp.end()){
            Node *current = it->second;
            current->value = value;
            moveToHead(current);
            return;
        }

        if(mp.size() == cap){
            deleteAtTail();
        }

        insertAtHead(key, value);
    }

private:

    void moveToHead(Node *current){
        if(head == current)
            return;

        if(tail == current){
            tail = tail->prev;
            tail->next = nullptr;
        }

        else{
            Node *temp1 = current->prev;
            Node *temp2 = current->next;
            temp1->next = temp2;
            temp2->prev = temp1;
        }

        current->next = head;
        head->prev = current;
        head = current;
    }
};
