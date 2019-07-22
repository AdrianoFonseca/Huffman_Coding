#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <math.h> 
#include "bitstream.hpp"

using namespace std;


long int bit_set2long(bitset<8*sizeof(long int)> in)
{
    auto str = in.to_string();
    reverse(str.begin(), str.end());
    auto y = bitset<8*sizeof(long int)>(str);
    return y.to_ulong();
}

char bit_set2char(bitset<8> in)
{
    auto str = in.to_string();
    reverse(str.begin(), str.end());
    auto y = bitset<8>(str);
    return (char)y.to_ulong();
}
 
int main()
{
    /* a priority queue normalmente ordena de maior para menor, com o pq_compare ela faz o contrario*/
    priority_queue< Node* ,vector< Node* >,pq_compare> tabela_huffman;
    

    map<char,long int> count;

    ifstream input;
    
    input.open("out.txt",ios::binary);
    if (!input)
    {
        cerr << "error: deu ruim\n";
        return -1;
    }
    
    long int num_pair;
    input.read(reinterpret_cast<char*>(&num_pair), sizeof(long int));
    bitset<8*sizeof(long int)> d(num_pair);
    num_pair = bit_set2long(d);

    char caracter;
    long int freq;
    long int total = 0;

    for (int i = 0; i < num_pair;i++)
    {
        input.read(&caracter, sizeof(char));
        input.read(reinterpret_cast<char*>(&freq), sizeof(long int));
        bitset<8*sizeof(long int)> d(freq);
        bitset<8> f(caracter);

        freq = bit_set2long(d);
        caracter = bit_set2char(f);

        count[caracter] = freq;
        total += freq;
    }

    create_queue(count,tabela_huffman);

    /*Combinar dois menores e criar arvore binaria*/ 
    create_bin_tree(tabela_huffman);
    //printBT(tabela_huffman.top());

    ofstream decoded;
    
    decoded.open("decoded.txt");

    Bit_reader instream (input);
    Bit_writer ostream (decoded);

    long int lidos = 0;
    int pos = 0;
    bool out;
    auto root = tabela_huffman.top();
    auto next = root;
    while(lidos < total)
    {
        if (pos == 8)
        {
            pos = 0;
        }
        out = instream.find_bit(pos);
        pos ++;
        
        next = ostream.write_decode(out, next, root, lidos);
    }

    input.close();

}