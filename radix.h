
#ifndef REDIXSORT_H
#define REDIXSORT_H

#define MAX_WORDS 1000000
#define MAX_LENGTH 100
#define MAX_CHARS 256
#define MAX_INPUT_SIZE 100
#define MAX_TOP 2000
#define INITIAL_CAPACITY 10

void radixsort_numerico(int vetor[], int tamanho);
void radixsort_strings1(char **palavras, int tamanho);
void radixsort_strings2(char palavras[][MAX_LENGTH], int tamanho);

#endif
