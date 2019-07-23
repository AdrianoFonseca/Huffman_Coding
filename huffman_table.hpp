#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>

class Node{
    public:
    char caracter;
    long int freq;
    Node *left, *right;
    std::vector <bool> codigo;

    Node(char a,long int b){
       caracter = a;
       freq = b;
    }
};

/*Para usar no priority queue*/

class pq_compare{
public:
	bool operator() (const Node* primeiro,const Node* segundo)
    {
        if(primeiro->freq == segundo->freq)
        {
            /* Variancia Minima */
            if(primeiro->caracter == '_'){
                return(true);
            }
            else
            {
                return(false);
            }
        }
        return(primeiro->freq > segundo->freq);
    }
};

void create_queue(std::map<char,long int>  & map_in ,std::priority_queue< Node* ,std::vector< Node* >,pq_compare> & queue)
{
    for (auto caracter: map_in)
    {
        auto current = new Node(caracter.first, caracter.second);
        current->left = nullptr;
        current->right = nullptr;

        queue.push(current);
    }
}

void create_bin_tree(std::priority_queue< Node* ,std::vector< Node* >,pq_compare> & queue)
{
    while (queue.size() >= 2)
	    {
		    auto primeiro = queue.top();
            queue.pop();
            auto segundo = queue.top();
            queue.pop();

            auto interno = new Node('_', primeiro->freq + segundo->freq);
            interno->left  = primeiro;
            interno->right = segundo;

            queue.push(interno);
	}
}
void code_from_tree(Node* node,std::map<char,std::vector <bool> >& code)
{
    if (node == nullptr)
        return;
    if(node->left != nullptr)
    {
        node->left->codigo = node->codigo;
        node->left->codigo.push_back(false);
    }
     if(node->right != nullptr)
    {
        node->right->codigo = node->codigo;
        node->right->codigo.push_back(true);
    }
    code_from_tree(node->left,code);
    code_from_tree(node->right,code);

    if(node->caracter != '_')
    {
        code[node->caracter] = node->codigo;
    }
}

void delete_tree(Node* node)
{
    if (node == NULL) return;
    delete_tree(node->left);
    delete_tree(node->right);

    free(node);
}

void print_tree(const std::string & prefix, const Node* node, bool isLeft)
    {
        if( node != nullptr )
        {
            std::cout << prefix;

            std::cout << (isLeft ? "├──" : "└──" );

            // print the value of the node
            if(node->caracter != '_')
            {
                std::cout << node->caracter << std::endl;
            }
            else
            {
                std::cout << "  " << std::endl;
            }


            // enter the next tree level - left and right branch
            print_tree( prefix + (isLeft ? "│   " : "    "), node->left, true);
            print_tree( prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

void print_tree(const Node* node)
{
    print_tree("", node, false);
}