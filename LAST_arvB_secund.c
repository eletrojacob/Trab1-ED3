#include "functions.c"

Node *criarNo();
void liberarNo(Node *no);
void split(char* newkey, int newkeyChildRRN, Node page, char* promoKey, int promoChild, Node newpage, Cabecalho* reg_cabecalho);
int insert(int currentRRN, char key[], char **promoKey, int promoChild, Cabecalho* reg_cabecalho);
void imprimirArvoreB(Node *raiz, int nivel);
void search(int RRN, char key, int foundRRN, int foundPos);

Node* criarNo(){

    Node* novoNo = (Node*) malloc(sizeof(Node));

    for (int i = 0; i < TAM_ORDEM; i++){
        novoNo->filhos[i] = - 1;
    }

    novoNo->nroChavesNo = 0;
    novoNo->alturaNo = 1;
    novoNo->RRNdoNo = 0; // Definindo o valor inicial do RRN do nó

    return novoNo;
}

void set_cabecalho(Cabecalho* reg_cabecalho, char status, int noRaiz, int RRNproxNo){
    (reg_cabecalho->status) = status;
    (reg_cabecalho->noRaiz) = noRaiz;
    (reg_cabecalho->RRNproxNo) = RRNproxNo;
}

int conta_registros(FILE* arq){
    char buffer;
    int counter = 0;
    fseek(arq, 205, SEEK_SET);
    while(fread(&buffer, sizeof(char), 205, arq) != 0){
        counter++;
    }
    return counter;
}

void driver(){
    FILE* file_indice = fopen("arvB.bin", "rb+");
    FILE* file_dados = fopen("binary.bin", "rb");

    Cabecalho reg_cabecalho;
    set_cabecalho(&reg_cabecalho, '0', -1, 0);

    //char empty_key[55];
    char* empty_key = (char*) malloc(55*sizeof(char));
    char lixo_cabecalho[196];
    int RRNROOT;
    memset(empty_key, '$', 55);
    empty_key[54] = '\0';

    memset(lixo_cabecalho, '$', 196);
    lixo_cabecalho[195] = '\0';

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
    fwrite(lixo_cabecalho, 1, 196, file_indice); // AQUI TEM QUE TER &?

    for (int i = 0; i < TAM_ORDEM - 1; i++){
        RRNROOT = reg_cabecalho.RRNproxNo;
        strcpy((root->chaves[i]).chave, empty_key);
    }
    char chave[55];

    strcpy(chave, funcionalidade4("binary.bin", 0).nomeTecnologiaOrigem);
    strcat(chave, funcionalidade4("binary.bin",0).nomeTecnologiaDestino);
    strcpy((root->chaves[0]).chave, chave);
    root->chaves[0].RRN = 0;
    root->nroChavesNo++;

    int counter = 1;
    while (funcionalidade4("binary.bin", counter).nomeTecnologiaOrigem != NULL){
        int promoChild = -1;
        if (insert(RRNROOT, chave, &empty_key, promoChild, &reg_cabecalho) == 1){
            reg_cabecalho.RRNproxNo++;
        }
    }
    free(aux);
    fclose(file_indice);
}

int ins_page(char* key, int child, Node* page){
    int i;

    for (i = page->nroChavesNo; strcmp(key, page->chaves[i - 1].chave) < 0  && i > 0; i--){
        page->chaves[i] = page->chaves[i-1];
        page->filhos[i+1] = child;
    }
}

int insert(int currentRRN, char key[], char** promoKey, int promoChild, Cabecalho* reg_cabecalho){ // lembrar de colocar o arquivo como parametro

    // PROMOTION = 1
    // NO PROMOTION = 0
    // ERROR = -1

    Node page; // precisa ser ponteiro? Precisa fazer alocação dinamica?
    Node newpage;
    //char PBkey[55];
    char* PBkey = (char*)malloc(55*sizeof(char));
    memset(PBkey, '$', 55);
    PBkey[55] = '\0';
    int PBRRN;

    //char* aux; // vetor de chars (string) para armazenar os chars de um campo
    //char** vec; // vetor dinâmico de strings para armazenar os campos de um registro
    //vec = malloc(55*sizeof(char));


    FILE* arq = fopen("binary.bin", "rb");


    if (currentRRN = -1){
        strcpy(*promoKey, key);
        promoChild = -1;
        return 1;
    }
    else{
        //char* str = malloc(256*sizeof(char)); // vetor dinamico ao qual serão atribuídas as strings de cada linha do arquivo
        fseek(arq, (currentRRN+1)*205, SEEK_SET);
        //fgets(str, 256, arq);

        //fread(buffer, sizeof, número, ponteiro)
        // Lembrar que essa estrutura que será não existe nas condições iniciais do programa
        fread(&page.nroChavesNo, 4, 1, arq);
        fread(&page.alturaNo, 4, 1, arq);
        fread(&page.RRNdoNo, 4, 1, arq);
        for (int k = 0; k < TAM_ORDEM - 2; k++){
            fread(&page.filhos[k], 4, 1, arq);
            fread(&page.chaves[k].chave, 55, 1, arq);
            fread(&page.chaves[k].RRN, 4, 1, arq);
        }
        fread(&page.filhos[2], 4, 1, arq);
        fread(&page.chaves[2].chave, 55, 1, arq);
        fread(&page.chaves[2].RRN, 4, 1, arq);
        fread(&page.filhos[3], 4, 1, arq);

        // < 0, str1 < str2
        // > 0, str1 > str2
        //(str1, str2)

        int pos = 0;
        for (int k = 0; k <= 2; k++){
            if (strcmp(key, page.chaves[k].chave) > 0){
                //if (k < 2)
                    pos++; // será que é isso mesmo? pos pode assumir valores como 3 ou -1??
            }
            else{
                if (strcmp(key, page.chaves[k].chave) < 0){
                        pos = k - 1;
                }
                else{
                    return -1;
                }
            }
        }
        int value = insert(page.filhos[pos], key, &PBkey, PBRRN, reg_cabecalho); // definiu o PBRRN anteriormente??

        if (value == 0 || value == -1){
            return value;
        }
        else{
            if (page.nroChavesNo < TAM_ORDEM - 1){
                ins_page(PBkey, PBRRN, &page);
                //btwrite?
                return 0;
            }
            else{
                // split
                fseek(arq, (currentRRN+1)*205, SEEK_SET); // fseek está certo mesmo?
                fwrite(&page.nroChavesNo, 4, 1, arq);
                fwrite(&page.alturaNo, 4, 1, arq);
                fwrite(&page.RRNdoNo, 4, 1, arq);
                // Pi, Ci, PRi
                for (int w = 0; w < TAM_ORDEM - 2; w++){
                    fwrite(&page.filhos[w], 4, 1, arq);
                    fwrite(&page.chaves[w].chave, 1, 55, arq);
                    fwrite(&page.chaves[w].RRN, 4, 1, arq);
                }
                fwrite(&page.filhos[2], 4, 1, arq);
                fwrite(&page.chaves[2].chave, 1, 55, arq);
                fwrite(&page.chaves[2].RRN, 4, 1, arq);
                fwrite(&page.filhos[3], 4, 1, arq);


                fseek(arq, 0, SEEK_END); // fseek está certo mesmo?
                fwrite(&newpage.nroChavesNo, 4, 1, arq);
                fwrite(&newpage.alturaNo, 4, 1, arq);
                fwrite(&newpage.RRNdoNo, 4, 1, arq);
                // Pi, Ci, PRi
                for (int w = 0; w < TAM_ORDEM - 2; w++){
                    fwrite(&newpage.filhos[w], 4, 1, arq);
                    fwrite(&newpage.chaves[w].chave, 1, 55, arq);
                    fwrite(&newpage.chaves[w].RRN, 4, 1, arq);
                }
                fwrite(&newpage.filhos[2], 4, 1, arq);
                fwrite(&newpage.chaves[2].chave, 1, 55, arq);
                fwrite(&newpage.chaves[2].RRN, 4, 1, arq);
                fwrite(&newpage.filhos[3], 4, 1, arq);

                return 1;
            }
        }
    }

    free(PBkey);
    fclose(arq);

}


void liberarNo(Node *no){
	free(no);
}


void split(char* newkey, int newkeyChildRRN, Node page, char* promoKey, int promoChild, Node newpage, Cabecalho* reg_cabecalho){
    
    // newkey = nova chave a ser inserida
    // newkeyChildRRN = filho a direita da nova chave a ser inserida
    // page = página de disco corrente
    // promoKey = chave promovida
    // promoChild = filho a direita da chave promovida
    // newpage = nova página de disco

    //set_cabecalho();

    typedef struct tmpPage{
        int nroChavesNo;
        int alturaNo;
        int RRNdoNo;
        int filhos[TAM_ORDEM+1]; // Pi, RRN dos nós filhos
        key chaves[TAM_ORDEM]; // Ci
    } TmpPage;

    TmpPage tmpPage;
	
    tmpPage.alturaNo = page.alturaNo;

	for (int i = 0; i < TAM_ORDEM - 1; i++){
        strcpy(tmpPage.chaves[i].chave, page.chaves[i].chave);
	}
	//tmpPage.nroChavesNo = TAM_ORDEM - 1;

    for (int y = 0; y < TAM_ORDEM; y++){
        tmpPage.filhos[y] = page.filhos[y];
    }

    int j;
    for (j = TAM_ORDEM - 1; strcmp(newkey, tmpPage.chaves[j - 1].chave) < 0 && j > 0; j--){
        strcpy(tmpPage.chaves[j].chave, tmpPage.chaves[j - 1].chave);
        tmpPage.filhos[j + 1] = tmpPage.filhos[j];
    }

    strcpy(tmpPage.chaves[3].chave, newkey);
    tmpPage.filhos[4] = newkeyChildRRN;


	/*for (int i = pai->nroChavesNo; i > indiceFilho; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }
    //pai->filhos[indiceFilho + 1] = novoFilho;
    pai->filhos[indiceFilho + 1] = novoFilho->RRNdoNo;

    for (int i = pai->nroChavesNo - 1; i >= indiceFilho; i--) {
        pai->chaves[i + 1] = pai->chaves[i]; // (verificar se esta realmente certo)
    }
    pai->chaves[indiceFilho] = filho->chaves[TAM_ORDEM - 1]; // (verificar se realmente ta certo)
    pai->nroChavesNo++;*/
}

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
