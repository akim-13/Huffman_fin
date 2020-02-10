#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "frequency.h"

#define ASCII_SIZE 256

typedef struct huffman_node huffman_node;
typedef struct linked_node linked_node;

struct huffman_node
{
	char chr;
	int cnt;
	huffman_node *left, *right;
};

struct linked_node
{
	linked_node *next;
	huffman_node *data;
};


void encode(FILE *input, FILE *output, frequency* freq);
void decode(FILE *input, FILE *output);

void encode_bin(FILE *input, FILE *output, frequency* freq);
void decode_bin(FILE *input, FILE *output);


huffman_node* make_huffman_tree(char *chrs, int *cnts, int amount);
huffman_node* make_huffman_node(char chr, int cnt);
int max_tree_depth(huffman_node *tree);
void print_huffman_tree(huffman_node* root);
void fill_lookup_table(char** lookup_table, huffman_node* node, char* tmp_string, int depth);
void free_huffman_tree(huffman_node* root);

linked_node* make_linked_node(huffman_node* data);
void add_linked_node(linked_node** root, linked_node* node);