#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <math.h>
#include "bitstream.hpp"

using namespace std;

int main(int argc, char* argv[])
{

    if(argc > 3)
    {
        cout << "somente dois argumentos (nome do arquivo a ser decodificado e nome do arquivo decodificado)" << endl;
        return 1;
    }

    /* a priority queue normalmente ordena de maior para menor, com o pq_compare ela faz o contrario*/
    priority_queue< Node* ,vector< Node* >,pq_compare> tabela_huffman;
    map<char,long int> count;

    ifstream input;

    input.open(argv[1],ios::binary);
    if (!input)
    {
        cerr << "error: deu ruim\n";
        return -1;
    }

    long int num_pair;
    input.read(reinterpret_cast<char*>(&num_pair), sizeof(long int));

    char caracter;
    long int freq;
    long int total = 0;

    for (int i = 0; i < num_pair;i++)
    {
        input.read(&caracter, sizeof(char));
        input.read(reinterpret_cast<char*>(&freq), sizeof(long int));

        count[caracter] = freq;
        total += freq;
    }

    create_queue(count,tabela_huffman);

    /*Combinar dois menores e criar arvore binaria*/
    create_bin_tree(tabela_huffman);

    //print_tree(tabela_huffman.top());

    ofstream decoded;
    Bit_reader instream (input);
    Bit_writer ostream (decoded);

    decoded.open(argv[2]);
    if (!decoded)
    {
        cerr << "error: deu ruim\n";
        return -1;
    }

    long int lidos = 0;
    int pos = 0;
    bool out;
    auto root = tabela_huffman.top();
    auto next = root;

    while(lidos < total)
    {
        out = instream.get_bit();
        next = ostream.write_decode(out, next, root, lidos);
    }

    //Remove Bin Tree
    delete_tree(tabela_huffman.top());

    return 0;
}

