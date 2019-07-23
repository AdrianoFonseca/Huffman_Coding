#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include "bitstream.hpp"
#include "huffman_table.hpp"

using namespace std;

int main(int argc, char* argv[])
{

    if(argc > 4)
    {
        cout << "(nome do arquivo a ser codificado, nome do arquivo codificado, printar arvore binaria (opcional)" << endl;
        return 1;
    }
    /* a priority queue normalmente ordena de maior para menor, com o pq_compare ela faz o contrario*/
    priority_queue< Node* ,vector< Node* >,pq_compare> tabela_huffman;
    map<char,long int> count;
    map<char,vector <bool> > codigo ;
    ifstream input;

    input.open(argv[1]);
    if (!input)
    {
        cerr << "error: deu ruim\n";
        return -1;
    }
    char c;
    while (input.get(c))          // loop getting single characters
    {
        count[uint8_t(c)]++;
    }

    create_queue(count,tabela_huffman);

    /*Combinar dois menores e criar arvore binaria*/
    create_bin_tree(tabela_huffman);

    /*Create Code*/
    code_from_tree(tabela_huffman.top(),codigo);

    if(argv[3])
    {
        print_tree(tabela_huffman.top());
    }

    //Remove Bin Tree
    delete_tree(tabela_huffman.top());

    ofstream ofile;
    Bit_writer outstream (ofile);

    ofile.open(argv[2]);
    if (!ofile)
    {
        cerr << "error: deu ruim\n";
        return -1;
    }
    input.clear();
    input.seekg(0);

    /*Adicionar maps em bits ao inicio do arquivo */
    outstream.write_map(count);

    for (char c; input.get(c);)
    {
        auto& code = codigo[c];
        for(auto i : code)
        {
            outstream.fill_buffer(i);
        }
    }
    outstream.dump_buffer();
    return 0;
}