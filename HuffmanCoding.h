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

    Node(char letter, int freq);
    static bool compare(Node *n1, Node *n2);
};

class HuffmanTree
{
public:
    Node *root;
    map<char, int> lettersFrequency;
    map<char, int> taken;
    priority_queue<Node *, vector<Node *>, decltype(&Node::compare)> remainingNodes;
    map<char, string> codesTable;
    int total = 0;

    HuffmanTree() : remainingNodes(&Node::compare) {}
    void initialize(string str);
    void insert(Node *node1, Node *node2);
    void calcStrFreq(string str);
    void printQueue();
    void printTree(Node *node, int indent);

    ////////////////////////////////////////////

    void codeBook();
    void traverse(Node *node, string str);
    ~HuffmanTree();
};