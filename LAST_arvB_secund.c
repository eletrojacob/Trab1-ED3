#include "functions.c"

Node *criarNo();
void liberarNo(Node *no);
void split(Node *pai, int indiceFilho, Node *filho);
Node *inserirNo(Node *no, int chave);
void imprimirArvoreB(Node *raiz, int nivel);
void search(int RRN, char key, int foundRRN, int foundPos);
int insert(int currentRRN, char key[], char promoKey[], int promoChild);

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
    FILE* file_indice = fopen("arvB.bin", "rb+");
    FILE* file_dados = fopen("binary.bin", "rb");

    Cabecalho reg_cabecalho;
    reg_cabecalho.status = '0';
    reg_cabecalho.noRaiz = -1;
    reg_cabecalho.RRNproxNo = 0;


    char empty_key[55];
    int RRNROOT;
    memset(empty_key, '$', 55);
    empty_key[55] = '\0';

    Node* root = criarNo();

    if (file_indice != NULL){
        printf("O arquivo existe.");
        fclose(file_indice);
    }
    else{
        printf("Arquivo inexistente. Criando arquivo...");
        if ((file_indice = fopen("arvB", "wb+")) != NULL){
            printf("arquivo criado.");
        }
        else{
            printf("Erro ao criar o arquivo");
        }
    }

    Registro* aux = (Registro*) malloc(sizeof(Registro));
    leitura_binario(aux, file_dados);

    fwrite(&reg_cabecalho.status, sizeof(char), 1, file_indice);
    fwrite(&reg_cabecalho.noRaiz, sizeof(int), 1, file_indice);
    fwrite(&reg_cabecalho.RRNproxNo, sizeof(int), 1, file_indice);

    for (int i = 0; i < TAM_ORDEM - 1; i++){
        RRNROOT = reg_cabecalho.RRNproxNo;
        strcpy((root->chaves[i]).chave, empty_key);
    }
    char chave[55];

    strcpy(chave, funcionalidade4("binary.bin", 0).nomeTecnologiaOrigem);
    strcat(chave, funcionalidade4("binary.bin",0).nomeTecnologiaDestino);
    strcpy((root->chaves[0]).chave, chave);

    int counter = 1;
    while (funcionalidade4("binary.bin", counter).nomeTecnologiaOrigem != NULL){
        int promoChild = -1;
        if (insert(RRNROOT, chave, empty_key, promoChild) == 1){
            reg_cabecalho.RRNproxNo++;
        }
    }
    free(aux);
    fclose(file_indice);
}

int insert(int currentRRN, char key[], char promoKey[], int promoChild){ // lembrar de colocar o arquivo como parametro

    // PROMOTION = 1
    // NO PROMOTION = 0
    // ERROR = -1

    Node* page; // precisa ser ponteiro? Precisa fazer alocação dinamica?
    Node* newpage = (Node*) malloc(sizeof(Node));
    int POS;
    char PBkey;
    int PBRRN;

    //char* aux; // vetor de chars (string) para armazenar os chars de um campo
    //char** vec; // vetor dinâmico de strings para armazenar os campos de um registro
    //vec = malloc(55*sizeof(char));


    FILE* arq = fopen("binary.bin", "rb");


    if (currentRRN = -1){
        strcpy(promoKey, key);
        promoChild = -1;
        return 1;
    }
    else{
        //char* str = malloc(256*sizeof(char)); // vetor dinamico ao qual serão atribuídas as strings de cada linha do arquivo
        fseek(arq, (currentRRN+1)*205, SEEK_SET);
        //fgets(str, 256, arq);

        // se der algum erro nesses fread, 
        // veja se o que esta dando problema é a primeira linha do csv ("NomeTecnologiaOrigem, grupo, (...)")

        //fread(buffer, sizeof, número, ponteiro)
        //nomeTecnologiaOrigem,grupo,popularidade,nomeTecnologiaDestino,peso
        fread(page->chaves, TAM_MAX*sizeof(char), 1, arq);
        char* aux = malloc(TAM_MAX*sizeof(char));
        fread(aux, TAM_MAX*sizeof(char), 1, arq);
        //strcat(page->chaves, aux);

    }

    fclose(arq);
    //free(aux);

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

    FILE* file;


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
