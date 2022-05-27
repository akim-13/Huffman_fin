#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman_tree.h"

int main(void)
{
    FILE* input = fopen("dna.txt", "rt");
    FILE* output = fopen("dna.output.bin", "wb");

    frequency* freq = read_frequency_from_text(input);
    huffman_node* tree = make_huffman_tree(freq->chrs, freq->cnts, freq->amount);

    print_huffman_tree(tree); 
    rewind(input);
    encode_bin(input, output, freq);

    free_frequency(freq);
    free_huffman_tree(tree); 
    fflush(output); // Errors on big files w/o this function
    fclose(output); 
    fclose(input);

    FILE* input0 = fopen("dna.output.bin", "rb");
    FILE* output0 = fopen("dna.output.bin.txt", "wt");

    decode_bin(input0, output0);

    fflush(output0); 
    fclose(output0);
    fclose(input0);

    return 0;
}
