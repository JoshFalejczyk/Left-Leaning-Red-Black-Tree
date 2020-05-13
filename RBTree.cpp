// Josh Falejczyk: CS201 Data Structures Library Phase 2
#include <iostream>
using namespace std;


template <typename keytype, typename valuetype>
class RBTree {

    struct Node { // Struct for each node of tree

        valuetype val;
        keytype key;
        bool color;
        Node* parent = nullptr;
        Node* left =  nullptr;
        Node* right = nullptr;
        int subtreeSize = 1;
        Node(keytype key, valuetype val) { // Constructor for Node

            this->key = key;
            this->val = val;
            this->color = true; // Red = true, black = false
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            subtreeSize = 1;

        }

    };

    public:
        RBTree(); // Constructor
        RBTree(keytype k[], valuetype V[], int s); // Overloaded constructor
        ~RBTree(); // Destructor
        RBTree(const RBTree &src); // Copy constructor
        RBTree<keytype, valuetype>& operator=(const RBTree &src); // Copy assignment operator
        valuetype* search(keytype k); // Traditional tree search
        void insert(keytype k, valuetype v); // Inserts node of key k and value v into tree
        int remove(keytype k); // Removes node of key k from tree
        int rank(keytype k); // Returns rank of key k in tree
        keytype select(int pos); // Returns key at position pos
        void split(keytype k, RBTree<keytype, valuetype>& T1, RBTree<keytype, valuetype>& T2); // Splits tree into T1 and T2 based on k
        int size(); // Returns number of nodes in tree
        void preorder(); // Print keys in preorder traversal
        void inorder(); // Print keys in inorder traversal
        void postorder(); // Print keys in postorder traversal

    private:
        
        Node* root; // Root of tree
        int numNodes; // Number of nodes in tree
        keytype kError; // Error variable to return for key
        valuetype vError; // Error variable to return for val
        Node* helperInsert(Node* n, keytype k, valuetype v); // Helps insert in recursive areas
        Node* helperRemove(Node* n, keytype k); // Helps delete node in recursive areas
        Node* deleteMin(Node* n); // Deletes the smallest node by key
        Node* moveRedLeft(Node* n); // Moves a red node left to prevent two successive red nodes
        Node* moveRedRight(Node* n); // Moves a red node right to prevent two successive red nodes
        Node* fixUp(Node* n); // Enforces LLRB Tree rules on the way up
        valuetype getVal(Node* temp, keytype k); // Searches for a node given key and returns its node
        keytype getMinKey(Node* temp); // Searches for and returns smallest key in subtree temp
        void helperSplit(Node* n, keytype k, RBTree<keytype, valuetype>& T1, RBTree<keytype, valuetype>& T2); // Helps distribute nodes in split 
        void helperPreorder(Node* n); // Helps preorder in recursive areas
        void helperInorder(Node* n); // Helps inorder in recursive areas
        void helperPostorder(Node* n); // Helps postorder in recursive areas
        Node* rotateLeft(Node* n); // Does a left rotation
        Node* rotateRight(Node* n); // Does a right rotation
        Node* colorFlip(Node* n); // Flips the color of n and its children
        int compare(keytype k1, keytype k2); // Helper function to compare keys
        void subtreeCheck(Node* rootNode); // Helper function that recalculates subtree size if necessary
        void helperDeepCopy(Node* &n, Node* nSrc); // Helper function that makes a deep copy in copy constructor
        void helperDestructor(Node* n); // Helper function that deletes every node in the tree for the destructor
        bool isRed(Node* n); // Checks to see if a node is red or not

};

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree() { // Constructor

    root = nullptr;
    numNodes = 0;

}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(keytype k[], valuetype V[], int s) { // Overloaded constructor

    root = nullptr;
    numNodes = 0;
    for (int i = 0; i < s; i ++) { // Iteratively insert nodes into RBTree

        insert(k[i], V[i]);

    }

}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::~RBTree() { // Destructor

    helperDestructor(root);

}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(const RBTree &src) { // Copy constructor

    numNodes = src.numNodes; // Copy over data
    kError = src.kError;
    vError = src.vError;
    helperDeepCopy(root, src.root);

}

template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>& RBTree<keytype, valuetype>::operator=(const RBTree<keytype, valuetype> &src) { // Copy assignment operator

        if (this != &src) { // Prevents self-assignment

            numNodes = src.numNodes; // Copy over data
            kError = src.kError;
            vError = src.vError;
            helperDeepCopy(root, src.root);

        }

        return *this;

}

template <typename keytype, typename valuetype>
valuetype* RBTree<keytype, valuetype>::search(keytype k) { // Traditional tree search

    Node* temp = root;
    while (temp != NULL) {

        if (compare(k, temp->key) == 0) { // Check for if node is found

            valuetype* result = &temp->val;
            return result;

        } else if (compare(k, temp->key) < 0) { // Move left

            temp = temp->left;

        } else if (compare(k, temp->key) > 0) { // Move right

            temp = temp->right;

        }

    }

    return NULL;

}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v) { // Inserts node of key k and value v into tree

    root = helperInsert(root, k, v); // Adding node to tree
    root->color = 0; // Ensuring root's color stays black
    numNodes ++;

    return;

}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::remove(keytype k) { // Removes node of key k from tree

    if (search(k) == NULL) { // Node of key k does not exist

        return 0;

    } 
    if (root && !root->left && !root->right) { // Root is sole node of tree, end preemptively
        
        delete root;
        root = NULL;
        numNodes --;
        return 1;

    }
    root = helperRemove(root, k); // Remove node from tree
    root->color = 0; // Ensuring root's color stays black
    numNodes --;

    return 1;

}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::rank(keytype k) { // Returns rank of key k in tree

    int count = 1; // Number of nodes visited, 1-based rank
    Node* temp = root;
    while (temp != NULL) { // Traverse tree

        if (k < temp->key) { // Move left

            temp = temp->left;

        } else if (k > temp->key) { // Move right

            if (temp->left) { // Account for all of left subtree

                count += 1 + temp->left->subtreeSize;
                temp = temp->right;

            } else {

                count ++;
                temp = temp->right;

            }

        } else if (temp->left) { // Node is found and has children

            return count + temp->left->subtreeSize;

        } else { // Node is found

            return count;

        }

    }

    return 0; // Node is never found

}

template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::select(int pos) { // Returns key at position pos

    int leftSubtreeSize;
    int count = pos; // Reverse of rank, 1-based rank
    Node* temp = root;
    while (temp != NULL) { // Traverse tree

        if (temp->left) { // Set leftSubtreeSize if possible

            leftSubtreeSize = temp->left->subtreeSize;

        } else {

            leftSubtreeSize = 0;

        }
        if (leftSubtreeSize + 1 == count) { // Node is found

            return temp->key;
        
        } else if (leftSubtreeSize < count) { // Move right

            temp = temp->right;
            count -= leftSubtreeSize + 1;

        } else { // Move left

            temp = temp->left;

        }

    }

    return kError; // Node is never found

}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::split(keytype k, RBTree<keytype, valuetype>& T1, RBTree<keytype, valuetype>& T2) { // Splits tree into T1 and T2 based on k

    helperSplit(root, k, T1, T2);

    return;

}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::size() { // Returns number of nodes in tree

    return numNodes; // numNodes is a private variable that keeps track of tree size in other functions

}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::preorder() { // Prints keys in preorder traversal

    if (root == NULL) { // RB Tree is empty

        return;

    } else {

        cout << root->key << " ";
        helperPreorder(root->left); // Move left
        helperPreorder(root->right); // Move right

    }

    return;

}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::inorder() { // Prints keys in inorder traversal

    if (root == NULL) { // RB Tree is empty

        return;

    } else {

        helperInorder(root->left); // Move left
        cout << root->key << " ";
        helperInorder(root->right); // Move right

    }

    return;

}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::postorder() { // Prints keys in postorder traversal

    if (root == NULL) { // RB Tree is empty

        return;

    } else {

        helperPostorder(root->left); // Move left
        helperPostorder(root->right); // Move right
        cout << root->key << " ";

    }

    return;

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::helperInsert(Node* n, keytype k, valuetype v){ // Helps insert in recursive areas

    if (n == NULL) { // RBTree is empty

        return new Node(k, v);

    }
    if (n->left && n->right && isRed(n->left) && isRed(n->right)) { // For 2-3-4 implementation rather than 2-3, split 4-Nodes on way down

        n = colorFlip(n);

    }
    if (compare(k, n->key) == 0) { // Node of same key found

        n->val = v;

    } else if (compare(k, n->key) < 0) { // Move left

        n->left = helperInsert(n->left, k, v);
        subtreeCheck(n);

    } else { // Move right

        n->right = helperInsert(n->right, k, v);
        subtreeCheck(n);

    }
    if (n->right && isRed(n->right)) { // Fix right-leaning red nodes on way up

        n = rotateLeft(n);

    }
    if (n->left && n->left->left && isRed(n->left) && isRed(n->left->left)) { // Prevents two successive red nodes

        n = rotateRight(n);

    }

    return n;

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::helperRemove(Node* n, keytype k) { // Helps delete node in recursive areas

    if (compare(k, n->key) < 0) { // Move left

        if (n->left && n->left->left && !isRed(n->left) && !isRed(n->left->left)) { // Check if red node needs to be moved left

            n = moveRedLeft(n);

        }
        if (n->left) {

            n->left = helperRemove(n->left, k);

        }

    } else {

        if (n->left && isRed(n->left)) { // Check if red needs to be rotated right

            n = rotateRight(n);

        }
        if (compare(k, n->key) == 0 && (n->right == NULL)) { // Node to be deleted is found, has no children

            delete n;
            return NULL;

        }
        if (n->right && n->right->left && !isRed(n->right) && !isRed(n->right->left)) { // Check if red node needs to be moved right

            n = moveRedRight(n);

        }
        if (compare(k, n->key) == 0 && n->right) { // Node to be deleted is found, has children

            n->key = getMinKey(n->right);
            n->val = getVal(n->right, n->key);
            n->right = deleteMin(n->right);

        } else { // Move right

            n->right = helperRemove(n->right, k);

        }

    }

    return fixUp(n); // Fix undesirable structures like 4-nodes on way up

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::deleteMin(Node* n) { // Deletes the smallest node by key

    if (n->left == NULL) { // Node is smallest key

        delete n;
        return NULL;

    }
    if (n->left && n->left->left && !isRed(n->left) && !isRed(n->left->left)) { // Move red link down if needed

        n = moveRedLeft(n);

    }
    n->left = deleteMin(n->left); // Move left

    return fixUp(n); // Fix undesirable structures like 4-nodes on way up

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::moveRedLeft(Node* n) { // Moves a red node left to prevent two successive red nodes

    n = colorFlip(n);
    if (n->right && n->right->left && isRed(n->right->left)) {

        n->right = rotateRight(n->right);
        n = rotateLeft(n);
        n = colorFlip(n);

    }

    return n;

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::moveRedRight(Node* n) { // Moves a red node right to prevent two successive red nodes

    n = colorFlip(n);
    if (n->left && n->left->left && isRed(n->left->left)) {

        n = rotateRight(n);
        n = colorFlip(n);

    }

    return n;

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::fixUp(Node* n) { // Enforces LLRB Tree rules on the way up

    if (n->right && isRed(n->right)) { // Rotate left to ensure left-leaning

        n = rotateLeft(n);

    }
    if (n->left && n->left->left && isRed(n->left) && isRed(n->left->left)) {

        n = rotateRight(n);

    }
    if (n->left && n->right && isRed(n->left) && isRed(n->right)) { // Split 4-nodes

        n = colorFlip(n);

    }
    subtreeCheck(n);

    return n;

}

template <typename keytype, typename valuetype>
valuetype RBTree<keytype, valuetype>::getVal(Node* temp, keytype k) { // Searches for a node given key and returns its node

    while (temp != NULL) { // Traverse RB Tree

        if (compare(k, temp->key) == 0) { // Node is found, return its value

            return temp->val;

        } else if (compare(k, temp->key) < 0) {// Move left

            temp = temp->left;

        } else if (compare(k, temp->key) > 0) { // Move right

            temp = temp->right;

        }

    }

    return vError; // Empty error variable for if node is never fouond

}

template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::getMinKey(Node* temp) { // Searches for and returns smallest key in subtree temp

    while (temp->left != NULL) { // Move to leftmost child

        temp = temp->left;

    }
    if (temp == NULL) {

        return kError;

    } else {

        return temp->key; // Return leftmost child's key

    }

}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::helperSplit(Node* n, keytype k, RBTree<keytype, valuetype>& T1, RBTree<keytype, valuetype>& T2) { // Helps distribute nodes in split 

    if (n == NULL) { // RB Tree is empty

        return;

    } else { // Splits tree in inorder traversal

        helperSplit(n->left, k, T1, T2);
        if (n->key < k) {

            T1.insert(n->key, n->val);

        } else if (n->key >= k) {

            T2.insert(n->key, n->val);

        }
        helperSplit(n->right, k, T1, T2);

    }

    return;

}

template <typename keytype, typename valuetype> 
void RBTree<keytype, valuetype>::helperPreorder(Node* n) { // Helps preorder in recursive areas
    
    if (n == NULL) { // End-case for recursion

        return;

    } else {

        cout << n->key << " ";
        helperPreorder(n->left); // Move left
        helperPreorder(n->right); // Move right

    }

    return;

}

template <typename keytype, typename valuetype> 
void RBTree<keytype, valuetype>::helperInorder(Node* n) { // Helps inorder in recursive areas
    
    if (n == NULL) { // End-case for recursion

        return;

    } else {

        helperInorder(n->left); // Move left
        cout << n->key << " ";
        helperInorder(n->right); // Move right

    }

    return;

}

template <typename keytype, typename valuetype> 
void RBTree<keytype, valuetype>::helperPostorder(Node* n) { // Helps postorder in recursive areas
    
    if (n == NULL) { // End-case for recursion

        return;

    } else {

        helperPostorder(n->left); // Move left
        helperPostorder(n->right); // Move right
        cout << n->key << " ";

    }

    return;

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::rotateLeft(Node* n) { // Does a left rotation

    Node* temp = n->right;
    n->right = temp->left;
    temp->left = n;
    if (temp->left) temp->color = temp->left->color;
    temp->left->color = true; // Red
    subtreeCheck(temp->left);
    subtreeCheck(temp);
    
    return temp;

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::rotateRight(Node* n) { // Does a right rotation

    Node* temp = n->left;
    n->left = temp->right;
    temp->right = n;
    if (temp->right) temp->color = temp->right->color;
    temp->right->color = true; // Red
    subtreeCheck(temp->right);
    subtreeCheck(temp);
    
    return temp;

}

template <typename keytype, typename valuetype>
typename RBTree<keytype, valuetype>::Node* RBTree<keytype, valuetype>::colorFlip(Node* n) { // Flips the color of n and its children

    if (n) n->color = !n->color;
    if (n->left) n->left->color = !n->left->color;
    if (n->right) n->right->color = !n->right->color;

    return n;

}

template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::compare(keytype k1, keytype k2) { // Helper function to compare keys

    if (k1 == k2) { // Keys are the same

        return 0;

    } else if (k1 > k2) { // First key is greater

        return 1;

    }

    return -1; // Second key is greater
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::subtreeCheck(Node* rootNode) { // Helper function that recalculates subtree size if necessary
    
    if (rootNode) { // Checks every possible Node configuration and assigns appropriate subtree size

        if (rootNode->left && rootNode->right) {

            rootNode->subtreeSize = 1 + rootNode->left->subtreeSize + rootNode->right->subtreeSize;

        } else if (!rootNode->left && rootNode->right) {

            rootNode->subtreeSize = 1 + rootNode->right->subtreeSize;

        } else if (rootNode->left && !rootNode->right) {

            rootNode->subtreeSize = 1 + rootNode->left->subtreeSize;

        } else {

            rootNode->subtreeSize = 1;

        }

    }

    return;

}

template <typename keytype, typename valuetype> 
void RBTree<keytype, valuetype>::helperDeepCopy(Node* &n, Node* nSrc) { // Helper function that makes a deep copy in copy constructor
    
    if (nSrc == NULL) { // End-case for recursion

        n = NULL;
        return;

    } else {

        n = new Node(nSrc->key, nSrc->val); // Copy data
        n->color = nSrc->color;
        n->parent = nSrc->parent;
        n->left = nSrc->left;
        n->right = nSrc->right;
        n->subtreeSize = nSrc->subtreeSize;
        helperDeepCopy(n->left, nSrc->left); // Move left
        helperDeepCopy(n->right, nSrc->right); // Move right

    }

    return;

}

template <typename keytype, typename valuetype> 
void RBTree<keytype, valuetype>::helperDestructor(Node* n) { // Helper function that deletes every node in the tree for the destructor
    
    if (n == NULL) { // End-case for recursion

        return;

    } else {

        helperDestructor(n->left); // Move left
        helperDestructor(n->right); // Move right
        delete n;

    }

    return;

}

template <typename keytype, typename valuetype>
bool RBTree<keytype, valuetype>::isRed(Node* n) { // Checks to see if a node is red or not

    if (n == NULL) {

        return false;

    }
    return (n->color == 1); // Returns true if Node is red

}