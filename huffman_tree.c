#define _CRT_SECURE_NO_WARNINGS

#include "huffman_tree.h"

#define MAXN 256

huffman_node* make_huffman_tree(char *chrs, int *cnts, int amount)
{
	if (amount == 0)
	{
		return NULL;
	}
	if (amount == 1)
	{
		return make_huffman_node(chrs[0], cnts[0]);
	}
	linked_node* root = NULL;
	for (int i = 0; i < amount; i++)
	{
		huffman_node* h_node = make_huffman_node(chrs[i], cnts[i]);
		linked_node* l_node = make_linked_node(h_node);
		add_linked_node(&root, l_node);
	}
	linked_node* node0;
	linked_node* node1;
	huffman_node* new_hnode;
	linked_node* new_lnode;
	int sum;
	for (int i = 0; i < (amount - 1); i++)
	{
		node0 = root;
		node1 = root->next;
		root = node1->next;
		sum = (node0->data)->cnt + (node1->data)->cnt;
		new_hnode = make_huffman_node(EOF, sum);
		new_hnode->right = node0->data;
		new_hnode->left = node1->data;
		new_lnode = make_linked_node(new_hnode);
		add_linked_node(&root, new_lnode);
		free(node0);
		free(node1);
	}
	return root->data;
}

huffman_node* make_huffman_node(char chr, int cnt) 
{
	huffman_node* node = (huffman_node*)malloc(sizeof(huffman_node));
	node->chr = chr;
	node->cnt = cnt;
	node->left = NULL;
	node->right = NULL;
	return node;
}

int max_tree_depth(huffman_node *tree) 
{
	if ((tree->left == NULL) && (tree->right == NULL)) 
	{
		return 0;
	}
	int right_depth = 0;
	int left_depth = 0;
	if (tree->right != NULL) 
	{
		right_depth = max_tree_depth(tree->right);
	}
	if (tree->left != NULL) 
	{
		left_depth = max_tree_depth(tree->left);
	}
	if (left_depth > right_depth) 
	{
		return left_depth + 1;
	}
	return right_depth + 1;
}

void fill_lookup_table(char** lookup_table, huffman_node* node, char* tmp_string, int depth)
{
	if ((node->left) == NULL && (node->right) == NULL)
	{
		lookup_table[node->chr] = malloc((depth + 1) * sizeof(char));
		tmp_string[depth] = '\0';
		strcpy(lookup_table[node->chr], tmp_string);
		return;
	}
	if ((node->right) != NULL)
	{
		tmp_string[depth] = '1';
		fill_lookup_table(lookup_table, node->right, tmp_string, depth + 1);
	}
	if ((node->left) != NULL) 
	{
		tmp_string[depth] = '0';
		fill_lookup_table(lookup_table, node->left, tmp_string, depth + 1);
	}
}

void print_huffman_tree_additional(huffman_node* node, int level) 
{
	if (node->right != NULL)
	{
		print_huffman_tree_additional(node->right, level + 1);
	}
	for (int i = 0; i < level; i++)
	{
		printf("      ");
	}
	if (node->chr == '\n')
	{
		printf("\'\\n\' %i\n\n", node->cnt);
	}
	else if (node->chr == EOF) 
	{
		printf("\'EOF\' %i\n\n", node->cnt);
	}
	else {
		printf("\'%c\' %i\n\n", node->chr, node->cnt);
	}
	if (node->left != NULL) {
		print_huffman_tree_additional(node->left, level + 1);
	}
}

void print_huffman_tree(huffman_node* node)
{
	print_huffman_tree_additional(node, 0);
}

void free_huffman_tree(huffman_node* node)
{
	if (node->left == NULL && node->right == NULL) 
	{
		free(node);
	}
	else 
	{
		free_huffman_tree(node->left);
		free_huffman_tree(node->right);
		free(node);
	}
}

linked_node* make_linked_node(huffman_node* data)
{
	linked_node* node = (linked_node *)malloc(sizeof(linked_node));
	node->next = NULL;
	node->data = data;
	return node;
}

void add_linked_node(linked_node** root_poinet, linked_node* node)
{
	if ((*root_poinet) == NULL)
	{
		(*root_poinet) = node;
		node->next = NULL;
	}
	else if ((node->data)->cnt < ((*root_poinet)->data)->cnt)
	{
		node->next = (*root_poinet);
		(*root_poinet) = node;
	}
	else 
	{
		add_linked_node(&((*root_poinet)->next), node);
	}
}

void encode(FILE *input, FILE *output, frequency* freq)
{
	huffman_node* tree = make_huffman_tree(freq->chrs, freq->cnts, freq->amount);
	char* tmp_string = malloc((max_tree_depth(tree) + 1) * sizeof(char));
	char** lookup_table = malloc(MAXN * sizeof(char*));
	for (int i = 0; i < MAXN; i++)
	{
		lookup_table[i] = NULL;
	}
	fill_lookup_table(lookup_table, tree, tmp_string, 0);
	for (int i = 0; i < MAXN; i++)
	{
		if (!(lookup_table[i] == NULL))
			printf("%s\n", lookup_table[i]);
	}
	free(tmp_string);

	write_frequency(freq, output);
	int c = fgetc(input);
	while (c != EOF)
	{
		fputs(lookup_table[c], output);
		c = fgetc(input);
	}
	for (int i = 0; i < MAXN; i++)
	{
		if (lookup_table[i] != NULL)
		{
			free(lookup_table[i]);
		}
	}
	free(lookup_table);
	free_huffman_tree(tree);
}

void write_bit(FILE* output, char c, unsigned int* buffer, int* buffer_pos)
{
	int buf_size = sizeof(unsigned int) * 8;
	(*buffer) |= (c - '0') << (buf_size - (*buffer_pos) - 1);
	(*buffer_pos)++;
	if ((*buffer_pos) == buf_size)
	{
		fwrite(buffer, sizeof(unsigned int), 1, output);
		(*buffer) = 0;
		(*buffer_pos) = 0;
	}
}

void encode_bin(FILE *input, FILE *output, frequency* freq)
{
	huffman_node* tree = make_huffman_tree(freq->chrs, freq->cnts, freq->amount);
	char* tmp_string = malloc((max_tree_depth(tree) + 1) * sizeof(char));
	char** lookup_table = malloc(MAXN * sizeof(char*));
	for (int i = 0; i < MAXN; i++)
	{
		lookup_table[i] = NULL;
	}
	fill_lookup_table(lookup_table, tree, tmp_string, 0);
	free(tmp_string);
	unsigned int buffer = 0;
	int buffer_pos = 0;
	write_frequency_bin(freq, output);
	int c = fgetc(input);
	// int j = 0;
	while (c != EOF)
	{
		// printf("%s %i\n", lookup_table[c], buffer_pos);
		for (int j = 0; lookup_table[c][j] != '\0'; j++)
		{
			write_bit(output, lookup_table[c][j], &buffer, &buffer_pos);
		}
		c = fgetc(input);
		/* j++;
		if (j > 100) 
			break; */
	}
	if (buffer_pos != 0)
	{
		fwrite(&buffer, sizeof(unsigned int), 1, output);
	}
	for (int i = 0; i < MAXN; i++)
	{
		if (lookup_table[i] != NULL)
		{
			free(lookup_table[i]);
		}
	}
	free(lookup_table);
	free_huffman_tree(tree);
}

void decode(FILE *input, FILE *output)
{
	frequency* freq = read_frequency(input);
	huffman_node* tree = make_huffman_tree(freq->chrs, freq->cnts, freq->amount);
	huffman_node* tmp_node = tree;
	int c = fgetc(input);
	while (c != EOF)
	{
		if (c == '1')
		{
			tmp_node = tmp_node->right;
		}
		if (c == '0')
		{
			tmp_node = tmp_node->left;
		}
		if (tmp_node->left == NULL && tmp_node->right == NULL)
		{
			fputc(tmp_node->chr, output);
			tmp_node = tree;
		}
		c = fgetc(input);
	}
	free_frequency(freq);
	free_huffman_tree(tree);
}

int read_bit(FILE* input, unsigned int* buffer, int* buffer_pos)
{
	int buf_size = sizeof(unsigned int) * 8;
	if ((*buffer_pos) == buf_size)
	{
		fread(buffer, sizeof(unsigned int), 1, input);
		(*buffer_pos) = 0;
	}
	int result = ((*buffer) & (1 << (buf_size - (*buffer_pos) - 1))) > 0;
	(*buffer_pos)++;
	return result;
}

void decode_bin(FILE *input, FILE *output) 
{
	frequency* freq = read_frequency_bin(input);
	huffman_node* tree = make_huffman_tree(freq->chrs, freq->cnts, freq->amount);
	int n = tree->cnt;
	huffman_node* tmp_node = tree;
	unsigned int buffer;
	int buffer_pos = sizeof(unsigned int) * 8;
	for (int i = 0; i < n; i++) {
		while (!(tmp_node->left == NULL && tmp_node->right == NULL))
		{
			if (read_bit(input, &buffer, &buffer_pos)) {
				tmp_node = tmp_node->right;
			}
			else {
				tmp_node = tmp_node->left;
			}
		}
		fputc(tmp_node->chr, output);
		tmp_node = tree;
	}
	free_frequency(freq);
	free_huffman_tree(tree);
}
