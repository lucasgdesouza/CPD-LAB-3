/*
    Nome: Lucas Gomes de Souza
    Cartão: 00580466
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "radix.h"

typedef struct {
    char palavra[MAX_LENGTH];
    int contador;
} PalavraContador;


void le_palavras(const char *fileEntrada, char palavras[][MAX_LENGTH], int *tamanho) {
    FILE *file = fopen(fileEntrada, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    *tamanho = 0;
    while (fscanf(file, "%s", palavras[*tamanho]) != EOF && *tamanho < MAX_WORDS) {
        (*tamanho)++;
    }

    fclose(file);
}

void escreve_palavras_ordenadas(const char *fileSaida, char palavras[][MAX_LENGTH], int tamanho) {
    FILE *file = fopen(fileSaida, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < tamanho; i++) {
        fprintf(file, "%s\n", palavras[i]);
    }

    fclose(file);
}

void imprime_repeticoes(const char *fileEntrada, const char *fileSAIDA) {

    int i, j;
    int tamanho;
    char palavras[MAX_WORDS][MAX_LENGTH];

    le_palavras(fileEntrada, palavras, &tamanho);

    int *visitado = (int *)malloc(tamanho * sizeof(int));
    FILE *file = fopen(fileSAIDA, "w");

    for (i = 0; i < tamanho; i++) {
        visitado[i] = 0;
    }

    for (i = 0; i < tamanho; i++) {
        if (visitado[i] == 1) continue;

        int contador_palavra= 1;
        for (j = i + 1; j < tamanho; j++) {
            if (strcmp(palavras[i], palavras[j]) == 0) {
                contador_palavra++;
                visitado[j] = 1;
            }
        }
        fprintf(file,"%s %d\n", palavras[i], contador_palavra);
    }

    fclose(file);
    free(visitado);
}

char **le_palavras_digitadas(int *tamanho) {
    int capacidade = 10; // Capacidade inicial para 10 palavras
    char **palavras = malloc(capacidade * sizeof(char *));
    if (palavras == NULL) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LENGTH];
    *tamanho = 0;

    printf("Digite palavras (digite 'fim' para terminar):\n");

    while (fgets(buffer, MAX_LENGTH, stdin)) {
        // Remove a nova linha do final da entrada, se existir
        buffer[strcspn(buffer, "\n")] = '\0';

        // Usa strtok para dividir o buffer em palavras, usando espaço e Enter como delimitadores
        char *palavra = strtok(buffer, " ");
        while (palavra != NULL) {
            // Verifica se a palavra de saída foi digitada
            if (strcmp(palavra, "fim") == 0) {
                return palavras; // Sai da função e retorna as palavras
            }

            // Realoca memória se necessário
            if (*tamanho >= capacidade) {
                capacidade *= 2;
                palavras = realloc(palavras, capacidade * sizeof(char *));
                if (palavras == NULL) {
                    perror("Erro ao realocar memória");
                    exit(EXIT_FAILURE);
                }
            }

            // Aloca memória para a nova palavra e a copia para o array
            palavras[*tamanho] = malloc((strlen(palavra) + 1) * sizeof(char));
            if (palavras[*tamanho] == NULL) {
                perror("Erro ao alocar memória para a palavra");
                exit(EXIT_FAILURE);
            }
            strcpy(palavras[*tamanho], palavra);
            (*tamanho)++;

            // Pega a próxima palavra, se existir
            palavra = strtok(NULL, " ");
        }
    }

    return palavras;
}

void conta_repeticoes(const char *fileEntrada, const char *fileSaida) {
    FILE *entrada = fopen(fileEntrada, "r");
    FILE *saida = fopen(fileSaida,"w");

    if (entrada == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    PalavraContador *palavras = malloc(sizeof(PalavraContador) * 1);
    int tamanho = 0;
    int capacidade = 1;

    char buffer[MAX_LENGTH];
    while (fgets(buffer, MAX_LENGTH, entrada) != NULL) {
        // Remove a nova linha do final da palavra lida, se existir
        buffer[strcspn(buffer, "\n")] = '\0';

        // Verificar se a palavra já está no array
        int encontrada = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(palavras[i].palavra, buffer) == 0) {
                palavras[i].contador++;
                encontrada = 1;
                break;
            }
        }

        // Se a palavra não foi encontrada, adicioná-la ao array
        if (!encontrada) {
            if (tamanho == capacidade) {
                capacidade *= 2;
                palavras = realloc(palavras, capacidade * sizeof(PalavraContador));
                if (palavras == NULL) {
                    perror("Erro ao realocar memória");
                    exit(EXIT_FAILURE);
                }
            }
            strcpy(palavras[tamanho].palavra, buffer);
            palavras[tamanho].contador = 1;
            tamanho++;
        }
    }

    fclose(entrada);

    // Imprimir as palavras e suas contagens
    for (int i = 0; i < tamanho; i++) {
        fprintf(saida, "%s: %d\n", palavras[i].palavra, palavras[i].contador);
    }
    free(palavras);
}

void insere_ordenado(PalavraContador *palavras, int *tamanho, PalavraContador nova) {
    int i;


    if (*tamanho == MAX_TOP && nova.contador <= palavras[MAX_TOP - 1].contador) {
        return;
    }


    for (i = *tamanho - 1; (i >= 0 && palavras[i].contador < nova.contador); i--) {
        if (i < MAX_TOP - 1) {
            palavras[i + 1] = palavras[i];
        }
    }

    palavras[i + 1] = nova;


    if (*tamanho < MAX_TOP) {
        (*tamanho)++;
    }
}

void processa_contado(const char *filename, PalavraContador *palavras, int *tamanho) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_LENGTH];
    int valor;
    *tamanho = 0;

    while (fscanf(file, "%s %d", buffer, &valor) != EOF) {
        PalavraContador nova;
        strcpy(nova.palavra, buffer);
        nova.contador = valor;
        insere_ordenado(palavras, tamanho, nova);
    }

    fclose(file);
}


void top2000(const char *fileEntrada, const char *fileSaida) {
    PalavraContador palavras[MAX_TOP];
    int tamanho = 0;
    FILE *saida = fopen(fileSaida, "w");
    if (saida == NULL) {
            perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }


    processa_contado(fileEntrada, palavras, &tamanho);


    for (int i = 0; i < tamanho; i++){
            fprintf(saida, "%s %d\n", palavras[i].palavra, palavras[i].contador);
    }

    fclose(saida);
    return 0;
}

void organiza_e_salva(const char *entrada, const char *saida){

    char (*palavras)[MAX_LENGTH] = malloc(MAX_WORDS * sizeof(*palavras));
    if (!palavras) {
        perror("Erro ao alocar memória para palavras");
        exit(EXIT_FAILURE);
    }
    int tamanho;


    le_palavras(entrada, palavras, &tamanho);


    radixsort_strings2(palavras, tamanho);


    escreve_palavras_ordenadas(saida, palavras, tamanho);

    printf("************TAMANHO %s = %d **************\n",entrada, tamanho);
    free(palavras);
}

void topico1() {
    int tamanho;
    char **palavras = le_palavras_digitadas(&tamanho);

    radixsort_strings1(palavras, tamanho);
    printf("\nPalavras digitadas:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%s\n", palavras[i]);
        free(palavras[i]);
    }
    free(palavras);
}


void topico2() {
    organiza_e_salva("frankestein.txt","frankenstein_sorted.txt");
    organiza_e_salva("war_and_peace.txt", "war_and_peace_sorted.txt");

}

void topico3(){

    conta_repeticoes("frankenstein_sorted.txt", "frankenstein_counted.txt");
    conta_repeticoes("war_and_peace_sorted.txt", "war_and_peace_counted.txt");

}

void topico4(){
    top2000("frankenstein_counted.txt", "frankenstein_ranked.txt");
    top2000("war_and_peace_counted.txt", "war_and_peace_ranked.txt");
}

int main() {
    char input[100];
    int topico;

    do{ // do while que permite a execução em tópicos de cada txt: counted, sorted e ranked.
        printf("ordena palavras digitadas = 1\n");
        printf("counted frankestein e war and peace = 2\n");
        printf("Conta as repetições frankestein e war and peace = 3\n");
        printf("Top 2000 frankestein e war and peace = 4\n");
        printf("Encerrar o programa = 6\n");
        printf("--------------------------------------------------------------\n");
        printf("Topico: ");
        if (fgets(input, sizeof(input), stdin) != NULL){
            if (sscanf(input, "%d", &topico) == 1){
                printf("Voce digitou: %d\n", topico);
            }
            else{
                printf("Erro: Entrada invalida\n");
            }
        }
        else{
                printf("Erro ao ler a entrada\n");
        }
        switch(topico){
            case 1:
                topico1();
                break;
            case 2:
                topico2();
                break;
            case 3:
                topico3();
                break;
            case 4:
                topico4();
                break;
            default:
                system("cls");
                printf("Valor fora do escopo\n");
        }
    }while(topico !=6);
    system("cls");
    return 0;
}
