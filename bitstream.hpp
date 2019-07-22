#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "huffman_table.hpp"

class Bit_writer
{
	private:

	char buffer =0;
	std::ostream& os;
	int pos = 0;

	public:
	Bit_writer(std::ostream& os_) : os(os_){};
    ~Bit_writer() 
	{ 
		if(pos!= 0)
		{
			os.put(buffer);
		} 
	}

	void fill_buffer(bool bit)
	{
		if(pos == 8)
		{
			std::bitset<8> data(buffer);
			os.put(buffer);
			buffer = 0;
			pos = 0;
		}
		if(bit)
		{
			buffer |= 1 << pos;
			pos++;
		}
		else{
			pos++;
		}
	}

	void write_map(std::map<char,long int> count)
	{
		bool j = false;
		long int size = count.size();

		std::bitset<8*sizeof(long int)> b(size);

		for (int i = 8*sizeof(long int);i>0;i--)
		{
			fill_buffer(b[i-1]);
		}

		for (auto caracter: count)
		{
			std::bitset<8> c(caracter.first);
			std::bitset<8*sizeof(long int)> d(caracter.second);

			for (int i = 8;i>0;i--)
			{
				fill_buffer(c[i-1]);
			}
			for (int i = 8*sizeof(long int);i>0;i--)
			{
				fill_buffer(d[i-1]);
			}
		}
	}

	Node* write_decode(bool decode,Node * next, Node * root,long int &lidos)
	{
		if(decode == 0)
		{
			if (next->left->caracter == '_')
			{
				return (next -> left);
			}
			else
			{
				os.put(next->left->caracter);
				lidos ++;
				return(root);
			}
		}
		else
		{
			if (next->right->caracter == '_')
			{
				return (next -> right);
			}
			else
			{
				os.put(next->right->caracter);
				lidos ++;
				return(root);
			}
		}
		

	}

};

class Bit_reader
{
	private:

	char buffer = 0;
	std::ifstream& is;

	public:
	Bit_reader(std::ifstream& is_) : is(is_){};

	bool find_bit(int pos)
	{ 
		if(pos== 0)
		{
			is.get(buffer);
		} 
		return (buffer & 1 << pos);
	}
};