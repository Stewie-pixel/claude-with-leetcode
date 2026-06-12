/*
// Definition for a Node.
class Node {
    int val;
    Node next;
    Node random;

    public Node(int val) {
        this.val = val;
        this.next = null;
        this.random = null;
    }
}
*/

class Solution {

    public Node copyRandomList(Node head) {
        if (head == null) {
            return null;
        }

        // Map the deep copy node to the actual node on linked list
        Node curr = head;
        Map<Node, Node> map = new HashMap<>();

        while (curr != null) {
            Node copyNode = new Node(curr.val);
            map.put(curr, copyNode);
            curr = curr.next;
        }

        // Connect all deep copy node to establish a deep copy of  linked list
        curr = head;

        while (curr != null) {
            Node newNode = map.get(curr);

            if (curr.next != null) {
                newNode.next = map.get(curr.next);
            }

            if (curr.random != null) {
                newNode.random = map.get(curr.random);
            }

            curr = curr.next;
        }

        return map.get(head);
    }
}
