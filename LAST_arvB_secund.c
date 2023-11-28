#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


Node *criarNo();
void liberarNo(Node *no);
void split(Node *pai, int indiceFilho, Node *filho);
Node *inserirNo(Node *no, int chave);
void imprimirArvoreB(Node *raiz, int nivel);
void search(int RRN, char key, int foundRRN, int foundPos);

int insert(int currentRRN, char key, char promoKey, int promoLchild, char* inputString){

    // PROMOTION = 1
    // NO PROMOTION = 0
    // ERROR = -1

    Node* page; // precisa ser ponteiro? Precisa fazer alocação dinamica?
    Node* newpage = (Node*) malloc(sizeof(Node));
    int pos;
    char PBkey;
    int PBRRN;

    //char* aux; // vetor de chars (string) para armazenar os chars de um campo
    //char** vec; // vetor dinâmico de strings para armazenar os campos de um registro
    //vec = malloc(55*sizeof(char));


    FILE* arq = fopen(inputString, "rb");


    if (currentRRN = -1){
        promoKey = key;
        promoLchild = -1;
        return 1;
    }
    else{
        //char* str = malloc(256*sizeof(char)); // vetor dinamico ao qual serão atribuídas as strings de cada linha do arquivo
        fseek(arq, currentRRN, SEEK_SET);
        //fgets(str, 256, arq);

        // se der algum erro nesses fread, 
        // veja se o que esta dando problema é a primeira linha do csv ("NomeTecnologiaOrigem, grupo, (...)")

        //fread(buffer, sizeof, número, ponteiro)
        //nomeTecnologiaOrigem,grupo,popularidade,nomeTecnologiaDestino,peso
        fread(page->chaves, sizeof(char), 1, arq);


    }

}

Node* criarNo(){

    Node* novoNo = (Node*) malloc(sizeof(Node));

    for (int i = 0; i < TAM_ORDEM; i++){
        novoNo->filhos[i] = - 1;
    }

    novoNo->nroChavesNo = 0;
    novoNo->alturaNo = 0;
    novoNo->RRNdoNo = 0; // Defina o valor inicial do RRN do nó
    novoNo->RRNregistro = 0;

    return novoNo;
}

void driver(){
    FILE* file;
    FILE* file_arv;

    char key_init[55];
    int RRNROOT;
    memset(key_init, '$', 55);
    key_init[55] = '\0';

    Node* root = criarNo();

    file_arv = fopen("dados1.csv", "rb");

    if ((file = fopen("arvB", "rb")) != NULL){
        printf("O arquivo existe.");
        fclose(file);
    }
    else{
        if ((file = fopen("arvB", "wb")) != NULL){
            printf("arquivo criado.");
            for (int i = 0; i < TAM_ORDEM - 1; i++){
                strcpy(root->chaves[i].chave, key_init);
                RRNROOT = 0;
                while (root->chaves[i].chave != NULL){

                }

            }

            fclose(file);
        } 
        else{
            return;
        }
    }

}


void liberarNo(Node *no){
	free(no);
}


void split(Node *pai, int indiceFilho, Node *filho){
	Node* novoFilho = criarNo();
	novoFilho -> alturaNo = filho -> alturaNo;

	for (int i = 0; i < TAM_ORDEM - 1; i++){
		for (int j = 0; j < 55; j++){
			//novoFilho->chave[i][j] = filho -> chave[i+TAM_ORDEM][j];
            novoFilho->chaves[i].chave[j] = filho-> chaves[i].chave[j];
		}
	}
	novoFilho-> nroChavesNo = TAM_ORDEM - 1;

	if (filho->filhos[0] != -1){
		for (int i = 0; i < TAM_ORDEM; i++) {
            novoFilho->filhos[i] = filho->filhos[i + TAM_ORDEM];
            filho->filhos[i + TAM_ORDEM] = -1;
        }
	}

	for (int i = pai->nroChavesNo; i > indiceFilho; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }
    //pai->filhos[indiceFilho + 1] = novoFilho;
    pai->filhos[indiceFilho + 1] = novoFilho->RRNdoNo;

    for (int i = pai->nroChavesNo - 1; i >= indiceFilho; i--) {
        pai->chaves[i + 1] = pai->chaves[i]; // (verificar se esta realmente certo)
    }
    pai->chaves[indiceFilho] = filho->chaves[TAM_ORDEM - 1]; // (verificar se realmente ta certo)
    pai->nroChavesNo++;
} // >>>>>>>>>>>>>>>>>>>>>>>>> FIM DA FUNCAO DE INSERCAO <<<<<<<<<<<<<<<<<<<


/*Node *inserirNo(Node *no, int chave){

	int i = no->nroChavesNo - 1;

    if (no->filhos[0] == NULL) {
        // Nó é uma folha, inserir chave aqui
        while (i >= 0 && chave < no->chave[i]) {
            no->chave[i + 1] = no->chave[i];
            i--;
        }
        no->chave[i + 1] = chave;
        no->nroChavesNo++;
    } else {
        // Nó não é uma folha, encontrar filho apropriado
        while (i >= 0 && chave < no->chave[i]) {
            i--;
        }
        i++;

        // Verificar se o filho está cheio
        if (no->filhos[i]->nroChavesNo == 2 * T - 1) {
            dividirNo(no, i, no->filhos[i]);
            if (chave > no->chave[i]) {
                i++;
            }
        }
        no->filhos[i] = inserirNo(no->filhos[i], chave);
    }
    return no;

}*/

void search(int RRN, char key, int foundRRN, int foundPos){
    if (RRN = -1){
        printf("pagina não encontrada");
        return;
    }
    else{

    }
}


int main(){
    //driver();

	return 0;
}