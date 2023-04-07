#include <iostream>
#include <string>
#include <map>
#include <queue>
using namespace std;

class Node
{
public:
    char letter;
    int freq;
    Node *left, *right;

    Node() {}
    Node(char letter, int freq);
    static bool compare(Node *n1, Node *n2);
};

class HuffmanEncoding
{
public:
    Node *root;
    priority_queue<Node *, vector<Node *>, decltype(&Node::compare)> remainingNodes;
    map<char, string> codesTable;
    map<string, char> decompressCodesTable;
    int total = 0;

    HuffmanEncoding() : remainingNodes(&Node::compare) {}
    HuffmanEncoding(string inputFile, string outputFile, string tag);
    void insert(Node *node1, Node *node2);
    void buildTree();
    void buildTable(Node *node, string str);

    static int binaryToDecimal(string bin);

    void compress(string inputFile, string encodedFile);
    void decompress(string inputFile, string encodedFile);
};
