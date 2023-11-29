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
} Cabecalho;

typedef struct key{
	char chave[55];
	int RRN;
} key;

typedef struct Node{
	int nroChavesNo;
	int alturaNo;
	int RRNdoNo;
	int filhos[TAM_ORDEM]; // Pi, RRN dos nós filhos
	key chaves[TAM_ORDEM - 1]; // Ci
	int RRNregistro; // Pri, referencia para o registro correspondente no arquivo de dados, -1 se não houver
} Node;

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
