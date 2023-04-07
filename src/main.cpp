#include <iostream>
#include <string>
#include <map>
#include <queue>
#include "huffmanEncoding.h"
#include <iomanip>
#include <fstream>
#include <bitset>

using namespace std;

int main(int argc, char *argv[])

{
	if (argc != 4 || (argc == 4 && (string(argv[1]) != "-c" && string(argv[1]) != "-d")))
	{
		printf("[-] HELP:\n");
		printf("To Compress: %s -c <FILE-NAME> <NEW-FILE-NAME.bin>\n", string(argv[0]).c_str());
		printf("To Decompress: %s -d <FILE-NAME> <NEW-FILE-NAME.txt>\n", string(argv[0]).c_str());
	}

	else
	{

		HuffmanEncoding huffman(argv[2], argv[3], argv[1]);
	}

	return 0;
}
