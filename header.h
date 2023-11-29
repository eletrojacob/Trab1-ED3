#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_ORDEM 4
#define TAM_MAX 55
#define TAM_STR 100
#define TAM 500

typedef struct Cabecalho
{
	char status;
	int noRaiz;
	int RRNproxNo;
} Cabecalho; // tamanho = 1 + 8 = 9

typedef struct key{
	char chave[55];
	int RRN; // Pri, referencia para o registro correspondente no arquivo de dados, -1 se não houver
} key; // tamanho = 55 + 4 = 59

typedef struct Node{
	int nroChavesNo;
	int alturaNo;
	int RRNdoNo;
	int filhos[TAM_ORDEM]; // Pi, RRN dos nós filhos
	key chaves[TAM_ORDEM - 1]; // Ci
} Node; // tamanho de cada nó (registro do arq de indice) = 4 * 3 + 4 * TAM_ORDEM + 59 * (TAM_ORDEM - 1) = 12 + 16 + 177 = 205

typedef struct{
    char removido;
    int grupo;
    int popularidade;
    int peso;
    int tamanhoTecnologiaOrigem;
    char* nomeTecnologiaOrigem;
    int tamanhoTecnologiaDestino;
    char* nomeTecnologiaDestino;
} Registro; // Tipo heterogeneo para os atributos de cada registro

void leitura_binario(Registro *registros, FILE *arq);
Registro funcionalidade4(char* input_string, int n);
