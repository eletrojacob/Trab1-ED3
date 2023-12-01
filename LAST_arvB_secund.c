#include "functions.c"

Node *criarNo();
void liberarNo(Node *no);
void split(char* newkey, int newkeyChildRRN, Node page, char* promoKey, int promoChild, Node newpage, Cabecalho* reg_cabecalho);
int insert(Node** node, int currentRRN, char key[], char **promoKey, int promoChild, Cabecalho* reg_cabecalho, FILE* file_dados, FILE* file_indice);
void imprimirArvoreB(Node *raiz, int nivel);

Node* criarNo(){

    Node* novoNo = (Node*) malloc(sizeof(Node));

    for (int i = 0; i < TAM_ORDEM; i++){
        novoNo->filhos[i] = - 1;
    }

    novoNo->nroChavesNo = 1; // começa com 1 mesmo? Será que não é zero?
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
    fseek(arq, 205+1, SEEK_SET); // tem esse +1 mesmo?
    while(fread(&buffer, sizeof(char), 205, arq) != 0){
        counter++;
    }
    return counter;
}

void driver(Node** root, FILE* file_dados, FILE* file_indice){

    Cabecalho reg_cabecalho;
    set_cabecalho(&reg_cabecalho, '0', -1, 0); // status = '0', noRaiz = -1, RRNproxNo = 0

    // CRIANDO CHAVES VAZIAS E O ESPAÇO VAZIO DO CABECALHO
    //char empty_key[55];
    char* empty_key = (char*) malloc(55*sizeof(char));
    memset(empty_key, '$', 55);
    empty_key[54] = '\0';
    
    char lixo_cabecalho[196];
    memset(lixo_cabecalho, '$', 196);
    lixo_cabecalho[195] = '\0';
    
    int RRNROOT = 0;

    if (file_indice != NULL){ // (SE O ARQUIVO EXISTE)
        printf("O arquivo existe.");
        //fclose(file_indice);
    }
    else{
        // CRIACAO DO ARQUIVO BINARIO DE INDICE
        printf("Arquivo inexistente. Criando arquivo...");
        if ((file_indice = fopen("arvB.bin", "wb+")) != NULL){
            printf("arquivo criado.");
        }
        else{
            printf("Erro ao criar o arquivo");
        }
    }

    //  CONFIGURACAO INICIAL DO CABECALHO
    fwrite(&reg_cabecalho.status, sizeof(char), 1, file_indice);
    fwrite(&reg_cabecalho.noRaiz, sizeof(int), 1, file_indice);
    fwrite(&reg_cabecalho.RRNproxNo, sizeof(int), 1, file_indice);
    fwrite(lixo_cabecalho, 1, 196, file_indice); // AQUI TEM QUE TER &?
    //--------------------------------------------

    // INICIANDO A RAIZ COM CHAVES VAZIAS
    for (int i = 0; i < TAM_ORDEM - 1; i++){
        RRNROOT = reg_cabecalho.RRNproxNo;
        strcpy(((*root)->chaves[i]).chave, empty_key);
    }
    char chave[55];

    // INSERINDO A CHAVE DO RRN 0 DO ARQUIVO BIN DE DADOS NA RAIZ DA ARVORE
    strcpy(chave, funcionalidade4("binary.bin", 0).nomeTecnologiaOrigem);
    strcat(chave, funcionalidade4("binary.bin",0).nomeTecnologiaDestino);
    strcpy(((*root)->chaves[0]).chave, chave);
    (*root)->chaves[0].RRN = 0;
    (*root)->nroChavesNo++;

    // INSERINDO TODAS AS OUTRAS CHAVES NA ARVORE
    int counter = 1;
    // O PROGRAMA ESTÁ LERDO POR CAUSA DESSA PARTE ABAIXO
    while (funcionalidade4("binary.bin", counter).nomeTecnologiaOrigem != NULL){
        int promoChild = -1;
        // *************verificar se essa função insert está sendo executada mesmo sendo somente sendo utilizada pelo if()
        if (insert(root, RRNROOT, chave, &empty_key, promoChild, &reg_cabecalho, file_dados, file_indice) == 1){ // SE INSERT RETORNAR PROMOTION
            reg_cabecalho.RRNproxNo++;
        }
        counter++;
    }
    free(empty_key);
} // >>>>>>>>>>>>>>>>>>>> FIM DA FUNCAO DRIVER

int ins_page(char* key, int child, Node* page){
    int i;

    for (i = page->nroChavesNo; strcmp(key, page->chaves[i - 1].chave) < 0  && i > 0; i--){
        page->chaves[i] = page->chaves[i-1];
        page->filhos[i+1] = child;
    }
}

int insert(Node** node, int currentRRN, char key[], char** promoKey, 
            int promoChild, Cabecalho* reg_cabecalho, FILE* file_dados, FILE* file_indice){ // lembrar de colocar o arquivo como parametro

    // PROMOTION = 1
    // NO PROMOTION = 0
    // ERROR = -1

    Node page; // precisa ser ponteiro? Precisa fazer alocação dinamica?
    Node newpage;
    //char PBkey[55];
    char* PBkey = (char*)malloc(55*sizeof(char));
    memset(PBkey, '$', 55);
    PBkey[55] = '\0';
    int PBRRN, found, promovido;
    int pos = 0;

    if (currentRRN == -1){
        strcpy(*promoKey, key);
        promoChild = -1;
        return 1;
    }
    else{
        fseek(file_indice, (currentRRN+1)*205, SEEK_SET);

        //fread(buffer, sizeof, número, ponteiro)
        // Lembrar que essa estrutura que será não existe nas condições iniciais do programa
        fread(&page.nroChavesNo, 4, 1, file_indice);
        fread(&page.alturaNo, 4, 1, file_indice);
        fread(&page.RRNdoNo, 4, 1, file_indice);
        for (int k = 0; k < TAM_ORDEM; k++){
            fread(&page.filhos[k], 4, 1, file_indice);
            if (k < TAM_ORDEM - 1){
                fread(&page.chaves[k].chave, 55, 1, file_indice);
                fread(&page.chaves[k].RRN, 4, 1, file_indice);    
            }
        }


        // < 0, str1 < str2
        // > 0, str1 > str2
        //(str1, str2)
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
        int value = insert(node, page.filhos[pos], key, &PBkey, PBRRN, reg_cabecalho, file_dados, file_indice); // definiu o PBRRN anteriormente??

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
                fseek(file_indice, (currentRRN+1)*205, SEEK_SET); // fseek está certo mesmo?
                fwrite(&page.nroChavesNo, 4, 1, file_indice);
                fwrite(&page.alturaNo, 4, 1, file_indice);
                fwrite(&page.RRNdoNo, 4, 1, file_indice);
                // Pi, Ci, PRi
                for (int w = 0; w < TAM_ORDEM; w++){
                    fwrite(&page.filhos[w], 4, 1, file_indice);
                    if (w < TAM_ORDEM - 1){
                        fwrite(&page.chaves[w].chave, 1, 55, file_indice);
                        fwrite(&page.chaves[w].RRN, 4, 1, file_indice);
                    }
                }


                fseek(file_indice, 0, SEEK_END); // fseek está certo mesmo?
                fwrite(&newpage.nroChavesNo, 4, 1, file_indice);
                fwrite(&newpage.alturaNo, 4, 1, file_indice);
                fwrite(&newpage.RRNdoNo, 4, 1, file_indice);
                
                // Pi, Ci, PRi
                for (int w = 0; w < TAM_ORDEM; w++){
                    fwrite(&newpage.filhos[w], 4, 1, file_indice);
                    if (w < TAM_ORDEM - 1){
                        fwrite(&newpage.chaves[w].chave, 1, 55, file_indice);
                        fwrite(&newpage.chaves[w].RRN, 4, 1, file_indice);    
                    }
                }

                //&&&&&&&&&&&&&&&&&& isso aqui é realmente necessário? está no lugar certo?
                if (insert(node, currentRRN, key, &PBkey, promoChild, reg_cabecalho, file_dados, file_indice) == 1) {
                //if (insert(node, RRNROOT, chave, &empty_key, promoChild, reg_cabecalho, file_dados, file_indice) == 1)
                reg_cabecalho->RRNproxNo++;
                *node = criarNo(); // Cria um novo nó e atualiza o ponteiro root
                }

                return 1;
            }
        }
    }

    /*promovido = insert(node, currentRRN, key, &PBkey, page.filhos[pos], reg_cabecalho, file_dados, file_indice);
    if (!promovido){
        return 0;
    }
    if (page.nroChavesNo < TAM_ORDEM - 1){
        ins_page(PBkey, )
    }*/

    free(PBkey);
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
}

int busca(int RRN, char* key, int foundRRN, int foundPos, FILE* file_indice){

    // 1 = FOUND; -1 = NOT FOUND

    int noRaiz;
    Node page;

    if (RRN = -1){
        printf("pagina não encontrada");
        return -1;
    }
    
    fseek(file_indice, 1, SEEK_SET);

    fread(&RRN, 4, 1, file_indice);
    fseek(file_indice, (noRaiz+1)*205, SEEK_SET);
    fread(&page.nroChavesNo, 4, 1, file_indice);
    fread(&page.alturaNo, 4, 1, file_indice);
    fread(&page.RRNdoNo, 4, 1, file_indice);
    for (int w = 0; w < TAM_ORDEM; w++){
        fread(&page.filhos[w], 4, 1, file_indice);
            if (w < TAM_ORDEM - 1){
                fread(&page.chaves[w].chave, 1, 55, file_indice);
                fread(&page.chaves[w].RRN, 4, 1, file_indice);
            }
    }

    int i;
    for (i = 0; i < TAM_ORDEM - 1; i++){
        if (!strcmp(key, page.chaves[i].chave)){
            foundRRN = RRN;
            foundPos = i;
            return 1;
        }
    }

    return busca(page.filhos[i], key, foundRRN, foundPos, file_indice);
}


int main(){

    FILE* file_dados = fopen("binary.bin", "rb");
    FILE* file_indice = fopen("arvB.bin", "wb+");

    Node* root = criarNo(); // CRIANDO A RAIZ DA ARVORE (root = malloc(sizeof(Node)))
    //driver(&root, file_dados, file_indice);

    free(root);
    fclose(file_indice);
    fclose(file_dados);

	return 0;
}
