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

	void fill_buffer(bool bit)
	{
		if(pos == 8)
		{
			dump_buffer();
		}
		if(bit)
		{
			buffer |= 1 << pos;
		}
		pos++;

	}

	void dump_buffer()
	{
		os.write(&buffer,1);
		buffer = 0;
		pos = 0;
	}

	void write_map(std::map<char,long int> count)
	{
		bool j = false;
		long int size = count.size();

		std::bitset<8*sizeof(long int)> num_pair(size);

		for (int i = 0;i<8*sizeof(long int);i++)
		{
			fill_buffer(num_pair[i]);
		}

		for (auto caracter: count)
		{
			std::bitset<8> car(caracter.first);
			std::bitset<8*sizeof(long int)> freq(caracter.second);

			for (int i = 0;i<8;i++)
			{
				fill_buffer(car[i]);
			}
			for (int i = 0;i<8*sizeof(long int);i++)
			{
				fill_buffer(freq[i]);
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
				os.write(&next->left->caracter,1);
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
				os.write(&next->right->caracter,1);
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
	int pos = 0;
	std::ifstream& is;

	public:
	Bit_reader(std::ifstream& is_) : is(is_){};

	bool get_bit()
	{
		if(pos== 0)
		{
			is.read(&buffer, sizeof(char));
		}
		bool res = buffer & 1 << pos;

		pos = (pos + 1) % 8;

		return (res);
	}
};