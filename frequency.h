#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#define ASCII_SIZE 256

typedef struct frequency frequency;

struct frequency
{
	char* chrs;
	int* cnts;
	int amount;
};

frequency* read_frequency_from_text(FILE *input);
frequency* read_frequency(FILE* file);
frequency* read_frequency_bin(FILE* file);
frequency* init_frequency(int amount);

void write_frequency(frequency *data, FILE* file);
void write_frequency_bin(frequency *data, FILE* file);
void print_frequency(frequency *data);
void free_frequency(frequency *data);
