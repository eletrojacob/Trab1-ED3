#define TAM_ORDEM 4
#define TAM_MAX 55

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
	int filhos[TAM_ORDEM]; // RRN dos nós filhos
	key chaves[TAM_ORDEM - 1];
	int RRNregistro; // Pri, referencia para o registro correspondente no arquivo de dados, -1 se não houver
	int nroChavesNo;
	int alturaNo;
	int RRNdoNo;
} Node;
