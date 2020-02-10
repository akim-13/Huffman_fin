#define _CRT_SECURE_NO_WARNINGS
#include "frequency.h"

frequency* read_frequency_from_text(FILE *input) 
{
	int ascii_size = 256;
	int ascii_frequency[ASCII_SIZE];
	memset(ascii_frequency, 0, ASCII_SIZE * sizeof(int));
	int c = fgetc(input);
	while (c != EOF) {
		ascii_frequency[c]++;
		c = fgetc(input);
	}
	int non_empty_count = 0;
	for (int i = 0; i < ASCII_SIZE; i++)
	{
		if (ascii_frequency[i] != 0)
		{
			non_empty_count++;
		}
	}
	frequency* freq = init_frequency(non_empty_count);
	for (int i = 0, j = 0; i < ascii_size; i++)
	{
		if (ascii_frequency[i] != 0) {
			(freq->chrs)[j] = (char)i;
			(freq->cnts)[j] = ascii_frequency[i];
			j++;
		}
	}
	return freq;
}

frequency* read_frequency(FILE* file)
{
	int n;
	fscanf(file, "%i\n", &n);
	frequency* data = init_frequency(n);
	for (int i = 0; i < n; i++) {
		fscanf(file, "\'%c\':%i\n", &(data->chrs[i]), &(data->cnts[i]));
	}
	return data;
}

frequency* read_frequency_bin(FILE* file)
{
	int n;
	fread(&n, sizeof(int), 1, file);
	frequency* data = init_frequency(n);
	for (int i = 0; i < n; i++) {
		fread(&((data->chrs)[i]), sizeof(char), 1, file);
		fread(&((data->cnts)[i]), sizeof(int), 1, file);
	}
	return data;
}

void write_frequency(frequency* data, FILE* output) 
{
	fprintf(output, "%i\n", data->amount);
	for (int i = 0; i < (data->amount); i++) {
		fprintf(output, "\'%c\':%i\n", data->chrs[i], data->cnts[i]);
	}
}

void write_frequency_bin(frequency *data, FILE* file) 
{
	fwrite(&(data->amount), sizeof(int), 1, file);
	for (int i = 0; i < (data->amount); i++) {
		fwrite(&((data->chrs)[i]), sizeof(char), 1, file);
		fwrite(&((data->cnts)[i]), sizeof(int), 1, file);
	}
}

void print_frequency(frequency* data)
{
	write_frequency(data, stdout);
}

frequency* init_frequency(int amount) 
{
	frequency* data = malloc(sizeof(frequency));
	data->amount = amount;
	data->chrs = malloc(amount * sizeof(char));
	data->cnts = malloc(amount * sizeof(int));
	return data;
}

void free_frequency(frequency *data)
{
	free(data->chrs);
	free(data->cnts);
	free(data);
}
