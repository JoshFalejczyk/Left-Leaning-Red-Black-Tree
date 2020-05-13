#include <iostream>
using namespace std;
#include "RBTree.cpp"

int main(){
	string K[10] = {"A","B","C","D","E","F","H","I","J","K"};
	int V[10] = {10,9,8,7,6,5,4,3,2,1};
	
	RBTree<string,int> T1, T2(K,V,10);
	
	for(int i=0; i<10; i++) T1.insert(K[i],V[i]);
	// T1 and T2 should be identical trees
	
	cout << *(T2.search("C")) << endl;
	// Should output 8
	
	cout << T1.rank("C") << endl;
	//Should output 3
	
	cout << T1.select(3) << endl;
	//Should output C 
	
	T2.preorder();
	//Should output D B A C I F E H K J
	
	T2.inorder();
	//Should output	A B C D E F H I J K
	
	cout << T2.remove("G") << endl;
	//Should output 0
	
	cout << T2.remove("B") << endl;
	//Should output 1
	
	cout <<T2.rank("J") << endl;
	//Should output 
	
	T2.inorder();
	//Should output	A C D E F H I J K
	
	RBTree<string,int> L, R;
	T1.split("G", L, R);
	cout << R.rank("K") << endl;
	// Should output 4
	
    RBTree<int,int> X;
	for (int i=1;i<100000;i++) X.insert(i,i);
	for (int i=1;i<100000;i++) {
		if(X.rank(i) != i) cout << "Rank error" << endl;
		if(X.select(i) != i) cout << "Select error" << endl;
		if(*(X.search(i)) != i) cout << "Search error" << endl;
	}  
	//Should be no output and should take seconds, not minutes
	return 0;
}
