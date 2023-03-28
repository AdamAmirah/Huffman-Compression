#include <iostream>
#include <string>
#include <map>
#include <queue>
#include "HuffmanCoding.h"
#include <iomanip>
using namespace std;

int main()
{
	HuffmanTree ht;
	ht.initialize("amirah adam");
	if (ht.root != NULL)
	{
		cout << ht.root->freq;
	}
	else
	{
		cout << "Error: Root is NULL" << endl;
	}

	ht.printTree(ht.root, 0);

	ht.codeBook();

	return 0;
}
