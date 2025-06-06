#include "radix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void radixsort_numerico(int vetor[], int tamanho) {
    int i;
    int *b;
    int maior = vetor[0];
    int exp = 1;

    b = (int *)calloc(tamanho, sizeof(int));

    for (i = 0; i < tamanho; i++) {
        if (vetor[i] > maior)
    	    maior = vetor[i];
    }

    while (maior/exp > 0) {
        int balde[10] = { 0 };
    	for (i = 0; i < tamanho; i++)
            {
                balde[(vetor[i] / exp) % 10]++;
    	    }
    	for (i = 1; i < 10; i++)
            {
                balde[i] += balde[i - 1];
            }
    	for (i = tamanho - 1; i >= 0; i--)
            {
                b[--balde[(vetor[i] / exp) % 10]] = vetor[i];
            }
    	for (i = 0; i < tamanho; i++)
            {
                vetor[i] = b[i];
            }
    	exp *= 10;
    }

    free(b);
}

void radixsort_strings1(char **palavras, int tamanho) {
    char **temp = malloc(tamanho * sizeof(char *));
    int i, j, m;
    int balde[MAX_CHARS];
    int maxlen = 0;

    if (temp == NULL) {
        perror("Erro ao alocar mem�ria");
        exit(EXIT_FAILURE);
    }

    // Aloca cada string dentro de temp
    for (i = 0; i < tamanho; i++) {
        temp[i] = malloc(MAX_LENGTH * sizeof(char));
        if (temp[i] == NULL) {
            perror("Erro ao alocar mem�ria para temp");
            exit(EXIT_FAILURE);
        }
    }

    // Encontrar o comprimento da palavra mais longa
    for (i = 0; i < tamanho; i++) {
        if (strlen(palavras[i]) > maxlen)
            maxlen = strlen(palavras[i]);
    }

    for (m = maxlen - 1; m >= 0; m--) {
        memset(balde, 0, sizeof(balde));

        // Contagem das ocorr�ncias
        for (i = 0; i < tamanho; i++) {
            int charIndex = (m < strlen(palavras[i])) ? (unsigned char)palavras[i][m] : 0;
            balde[charIndex]++;
        }

        // Ajuste dos �ndices do balde
        for (i = 1; i < MAX_CHARS; i++) {
            balde[i] += balde[i - 1];
        }

        // Constru��o da lista tempor�ria
        for (i = tamanho - 1; i >= 0; i--) {
            int charIndex = (m < strlen(palavras[i])) ? (unsigned char)palavras[i][m] : 0;
            strcpy(temp[--balde[charIndex]], palavras[i]);
        }

        // C�pia da lista tempor�ria para palavras
        for (i = 0; i < tamanho; i++) {
            strcpy(palavras[i], temp[i]);
        }
    }

    // Libera a mem�ria alocada para temp
    for (i = 0; i < tamanho; i++) {
        free(temp[i]);
    }
    free(temp);
}

void radixsort_strings2(char palavras[][MAX_LENGTH], int tamanho) {
    char (*temp)[MAX_LENGTH] = malloc(tamanho * sizeof(*temp));
    int i, m;
    int balde[MAX_CHARS];
    int maxlen = 0;

    if (temp == NULL) {
        perror("Erro ao alocar mem�ria");
        exit(EXIT_FAILURE);
    }

    // Encontrar o comprimento da palavra mais longa
    for (i = 0; i < tamanho; i++) {
        if (strlen(palavras[i]) > maxlen)
            maxlen = strlen(palavras[i]);
    }

    for (m = maxlen - 1; m >= 0; m--) {
        memset(balde, 0, sizeof(balde));

        // Contagem das ocorr�ncias
        for (i = 0; i < tamanho; i++) {
            int charIndex = (m < strlen(palavras[i])) ? palavras[i][m] : 0;
            balde[charIndex]++;
        }

        // Ajuste dos �ndices do balde
        for (i = 1; i < MAX_CHARS; i++) {
            balde[i] += balde[i - 1];
        }

        // Constru��o da lista tempor�ria
        for (i = tamanho - 1; i >= 0; i--) {
            int charIndex = (m < strlen(palavras[i])) ? palavras[i][m] : 0;
            strcpy(temp[--balde[charIndex]], palavras[i]);
        }

        // C�pia de tempor�ria para palavras original
        for (i = 0; i < tamanho; i++) {
            strcpy(palavras[i], temp[i]);
        }
    }

    free(temp);
}
