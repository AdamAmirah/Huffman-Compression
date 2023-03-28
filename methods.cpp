#include "HuffmanCoding.h"
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <iomanip>

using namespace std;
Node::Node(char letter, int freq)
{
    this->letter = letter;
    this->freq = freq;
    this->left = NULL;
    this->right = NULL;
}

bool Node::compare(Node *n1, Node *n2)
{
    return n1->freq > n2->freq;
}

///////////////////////////////////////////////////////
void HuffmanTree::initialize(string str)
{

    calcStrFreq(str);
    for (int i = 0; i < str.size(); i++)
    {
        if (taken[str[i]] > 0)
            continue;
        Node *node = new Node(str[i], lettersFrequency[str[i]]);
        taken[str[i]]++;
        total += lettersFrequency[str[i]];
        remainingNodes.push(node);
    }
    // printQueue();

    while (!remainingNodes.empty())
    {
        Node *node1 = remainingNodes.top();
        remainingNodes.pop();
        Node *node2 = remainingNodes.top();
        remainingNodes.pop();
        insert(node1, node2);
        // break;
    }
}

void HuffmanTree::insert(Node *node1, Node *node2)
{
    Node *newNode = new Node('\0', node1->freq + node2->freq);
    newNode->left = node1;
    newNode->right = node2;

    if (total != node1->freq + node2->freq)
        remainingNodes.push(newNode);
    else
        root = newNode;
}
void HuffmanTree::calcStrFreq(string str)
{
    for (int i = 0; i < str.size(); i++)
        lettersFrequency[str[i]]++;
}

void HuffmanTree::printQueue()
{
    priority_queue<Node *, vector<Node *>, decltype(&Node::compare)> pq(&Node::compare);
    pq = remainingNodes;
    while (!pq.empty())
    {
        cout << pq.top()->letter << " " << pq.top()->freq << endl;
        pq.pop();
    }
}

void HuffmanTree::printTree(Node *node, int indent)
{
    if (node == NULL)
    {
        return;
    }
    if (node->right != NULL)
    {
        printTree(node->right, indent + 4);
    }
    if (indent > 0)
    {
        cout << setw(indent) << " ";
    }
    if (node->right != NULL)
    {
        cout << " /\n"
             << setw(indent) << " ";
    }
    cout << node->letter << ":" << node->freq << "\n ";
    if (node->left != NULL)
    {
        cout << setw(indent) << " "
             << " \\\n";
        printTree(node->left, indent + 4);
    }
}

HuffmanTree::~HuffmanTree()
{
    while (!remainingNodes.empty())
    {
        delete remainingNodes.top();
        remainingNodes.pop();
    }
}

void HuffmanTree::codeBook()
{
    traverse(root, "");
    for (auto code : codesTable)
    {
        cout << code.first << "  " << code.second << endl;
    }
}

void HuffmanTree::traverse(Node *node, string code)
{
    if (!node)
        return;
    if (node->letter != '\0')
        codesTable[node->letter] = code;

    traverse(node->left, code + "0");
    traverse(node->right, code + "1");
}