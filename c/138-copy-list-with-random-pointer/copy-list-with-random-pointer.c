// -----------------------------------------------------------------------------
// Problem: Copy List with Random Pointer
// Source : https://leetcode.com/problems/copy-list-with-random-pointer/
// Author : Ngoc-Tien To
// -----------------------------------------------------------------------------

// ---------------------------------- DESCRIPTION ----------------------------------
//
// A linked list is given where each node contains an integer value
// and a random pointer that could point to any node in the list or null.
//
// You must return a deep copy of the list.
// Each node in the new list should have the same value and random pointer structure
// as the original, but must be completely independent in memory.
//
// Example:
//   Input: head = [[7,null],[13,0],[11,4],[10,2],[1,0]]
//   Output: [[7,null],[13,0],[11,4],[10,2],[1,0]]
//
// ----------------------------------------------------------------------------------


// ---------------------------------- IDEA ----------------------------------
//
// 1. Traverse the original list and store each node in an array (pair array).
// 2. For each node, find the index of its random target node
//    and save that index in the same array.
// 3. Create a new list with the same values as the original list.
// 4. Connect all new nodes using the 'next' pointer.
// 5. Using the saved random indices, link the 'random' pointers in the new list.
// 6. Return the head of the newly created deep-copied list.
//
//  Time Complexity  : O(n²)  (due to nested search for random pointer)
//  Space Complexity : O(n)
// -----------------------------------------------------------------------------


struct Node {
    int val;
    struct Node *next;
    struct Node *random;
};

struct Pair{
    int index;
    struct Node *current;
};

struct Node *create(int val){
    // 1. Allocate a new node and initialize fields
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->val = val;
    newNode->next = NULL;
    newNode->random = NULL;
    return newNode;
}

struct Node* copyRandomList(struct Node* head) {
    // 1. Handle empty list case
    if(!head)
        return NULL;

    // 2. Create an array to store original nodes and random indices
    struct Pair *pair = (struct Pair *)malloc(sizeof(struct Pair) * (1e3));
    int size = 0;
	struct Node *temp = head;

    // 3. Traverse the original list and store each node
    while(temp){
        pair[size].index = -1;          // -1 means no random pointer
        pair[size].current = temp;      // store the current node pointer
        size++;
        temp = temp->next;
    }

    // 4. For each node, find the index of its random node (if exists)
    for(int i = 0; i < size; i++){
        if(pair[i].current->random){
            for(int j = 0; j < size; j++){
                if(pair[i].current->random == pair[j].current){
                    pair[i].index = j;  // save index of random target
                    break;
                }
            }
        }
    }

    // 5. Create a new node for each original node (copying only value)
    for(int i = 0; i < size; i++){
        int val = pair[i].current->val;
        pair[i].current = create(val);
    }

    // 6. Connect 'next' pointers in the new list
    for(int i = 0; i < size - 1; i++){
        pair[i].current->next = pair[i + 1].current;
    }

    // 7. Connect 'random' pointers using saved random indices
    for(int i = 0; i < size; i++){
        if(pair[i].index != -1){
            pair[i].current->random = pair[pair[i].index].current;
        }
    }
    
    // 8. Get the new head and free the temporary array
    struct Node *newHead = pair[0].current;
    free(pair);

    // 9. Return the deep copy of the list
    return newHead;
}
