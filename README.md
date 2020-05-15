# Left-Leaning-Red-Black-Tree

This was a project from Fall 2019 for my CS201 course.  It is a C++ implementation of a left-leaning red-black tree, popularized by Robert Sedgewick.  Though perhaps coding this in Java would have made things a bit easier, doing this in C++ taught me a lot about manual memory management in C++.  Nodes in the RBTree hold both key and data values, and a variety of functions are available.

Functions/Usage: 
1. Constructor (RBTree object constructor plus optional overloaded constructor for preloaded data)
2. Insert (inserts a Node into RBTree)
3. Remove (removes a Node from RBTree)
4. Search (traditional BST search that returns value given key)
5. Rank (returns rank of Node  in tree given its key)
6. Select/Order Statistics (returns key of Node given position in tree, 1-based)
7. Split (splits RBTree into two separate subtrees by a dividing key point)
8. Size (returns size of/number of nodes in tree)
9. Output (outputs RBTree using preorder, inorder, or postorder traversal, your choice)
10. Copy Constructor/Copy Assignment Operator (included for good practice of dynamic memory allocation)
11. Destructor (properly frees manually allocated memory within RBTree)

To Run (on Bash): 
1. Compile using "make" or "g++ -Wall 201MainPhase2.cpp".
2. Run using "./phase2" for the former compilation command, or "./a.exe" (or "./a.out" if on Mac) for the latter.

Credit: 
1. The concept of this project and 201MainPhase2.cpp test runner file were written and supplied by my professor, Dr. Brandon Dixon.
2. The RBTree.cpp file and Makefile were written by myself.
