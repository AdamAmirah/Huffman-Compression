#include "huffmanEncoding.h"
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <iomanip>
#include <fstream>
#include <bitset>

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

HuffmanEncoding::HuffmanEncoding(string inputFile, string outputFile, string tag) : remainingNodes(&Node::compare)
{

    if (tag == "-c")
    {
        ifstream ifs(inputFile, ios::in | ios::binary);

        if (!ifs.is_open())
        {
            cerr << "Error : Please check if the file exists";
            exit(1);
        }

        vector<int> lettersFrequency(255, 0);
        vector<int> taken(255, 0);

        char character;
        string str;

        while (ifs.get(character))
        {
            str += character;
            ++lettersFrequency[int(character)];
        }

        ifs.close();

        for (int i = 0; i < str.size(); i++)
        {
            if (taken[(int)str[i]] > 0)
                continue;
            Node *node = new Node(str[i], lettersFrequency[(int)str[i]]);
            ++taken[(int)str[i]];
            total += lettersFrequency[(int)str[i]];
            remainingNodes.push(node);
        }
        buildTree();
        buildTable(root, "");

        compress(inputFile, outputFile);
    }

    else if (tag == "-d")
    {
        decompress(inputFile, outputFile);
    }
}

void HuffmanEncoding::buildTree()
{
    while (!remainingNodes.empty())
    {
        Node *node1 = remainingNodes.top();
        remainingNodes.pop();
        Node *node2 = remainingNodes.top();
        remainingNodes.pop();
        insert(node1, node2);
    }
}

void HuffmanEncoding::insert(Node *node1, Node *node2)
{
    Node *newNode = new Node('\0', node1->freq + node2->freq);
    newNode->left = node1;
    newNode->right = node2;

    if (total != node1->freq + node2->freq)
        remainingNodes.push(newNode);
    else
        root = newNode;
}

void HuffmanEncoding::buildTable(Node *node, string code)
{
    if (!node)
        return;
    if (node->letter != '\0')
        codesTable[node->letter] = code;

    buildTable(node->left, code + "0");
    buildTable(node->right, code + "1");
}

int HuffmanEncoding::binaryToDecimal(string bin)
{
    int num = 0;
    int power = 1;
    for (int i = bin.size() - 1; i >= 0; i--)
    {
        if (bin[i] == '1')
        {
            num += power;
        }
        power *= 2;
    }

    return num;
}

////////////////////////////////////////////////////

void HuffmanEncoding::compress(string inputFile, string encodedFile)
{
    /*
     * create a header that contains :
     * 1) 1 byte for the number of characters in the codes table
     * 2) *FOR EACH CHARACTER IN THE CODES TABLE*
     *   2.1) 1 byte for the ASCII code of the character
     *   2.2) 1 byte for the length of the characters's code
     *   2.3) x bytes for the characters's code (depending on the length)
     * 3) add the added zeros at the end
     */

    string header = "";
    header += (char)codesTable.size();

    for (int i = 0; i <= 255; i++)
    {
        if (codesTable.find(char(i)) != codesTable.end())
        {
            string binSize = "";
            header += (char)i;
            header += (char)codesTable[char(i)].size();

            int trailingZeros = 8 - codesTable[char(i)].size() % 8;
            binSize += codesTable[char(i)] + string(trailingZeros, '0');
            for (int j = 0; j < binSize.size(); j += 8)
            {
                header += (char)binaryToDecimal(binSize.substr(j, 8));
            }
        }
    }

    ifstream ifs(inputFile, ios::in | ios::binary);

    if (!ifs.is_open())
    {
        cerr << "Error : Please check if the file exists";
        exit(1);
    }
    ofstream ofs(encodedFile, ios::out | ios::binary);

    if (!ofs.is_open())
    {
        cerr << "Error : Please check if the file exists";
        exit(1);
    }

    /*
     * Replace every character in the input file with the code from the table
     * Add zeros to the end of the end of the remaining bits
     */

    char character;
    string finalBin = "";
    string currBin = "";

    while (ifs.get(character))
    {
        currBin += codesTable[character];
        while (currBin.size() >= 8)
        {
            bitset<8> byte(currBin.substr(0, 8));
            finalBin += (char)(byte.to_ulong());
            currBin.erase(0, 8);
        }
    }
    int trailingZeros = 0;

    if (currBin.size() > 0)
    {
        trailingZeros = 8 - currBin.size();
        currBin += string(trailingZeros, '0');
        bitset<8> byte(currBin);
        finalBin += (char)(byte.to_ulong());
    }

    header += (char)trailingZeros;

    ofs << header << finalBin;

    ifs.close();
    ofs.close();

    cout << "COMPRESSING IS DONE\n";

    ifstream compressedFile(encodedFile, std::ios::binary | std::ios::ate);
    streampos fileSize = compressedFile.tellg();

    std::cout << "The size of the compressed file is " << fileSize << " bytes." << std::endl;
}
void HuffmanEncoding::decompress(string inputFile, string encodedFile)
{
    /*
     * Read the header content
     * Create the table
     * Remove the extra zeros
     * write the characters to the file
     */

    ifstream ifs(inputFile, ios::in | ios::binary);

    if (!ifs.is_open())
    {
        cerr << "Error : Please check if the file exists";
        exit(1);
    }
    ofstream ofs(encodedFile, ios::out | ios::binary);

    if (!ofs.is_open())
    {
        cerr << "Error : Please check if the file exists";
        exit(1);
    }

    string binCode = "";
    char byte;

    while (ifs.get(byte))
    {
        bitset<8> bits(byte);
        binCode += bits.to_string();
    }

    int counter = 0;
    int tableSize = binaryToDecimal(binCode.substr(0, 8));
    binCode.erase(0, 8);

    while (counter != (tableSize * 3))
    {

        char character = (char)binaryToDecimal(binCode.substr(0, 8));
        binCode.erase(0, 8);

        int codeSize = binaryToDecimal(binCode.substr(0, 8));
        int totalSize = codeSize + (8 - codeSize % 8);
        binCode.erase(0, 8);

        string binaryCode = binCode.substr(0, codeSize);
        binCode.erase(0, totalSize);

        decompressCodesTable[binaryCode] = character;

        counter += 3;
    }

    int trailingZeros = binaryToDecimal(binCode.substr(0, 8));
    binCode.erase(0, 8);

    int totalSize = binCode.size() - trailingZeros;
    string bin = "";
    for (int i = 0; i < totalSize; i++)
    {

        bin += binCode[i];
        if (decompressCodesTable.find(bin) != decompressCodesTable.end())
        {
            ofs << decompressCodesTable[bin];
            bin = "";
        }
    }

    ifs.close();
    ofs.close();

    cout << "DECOMPRESSING IS DONE\n";
}