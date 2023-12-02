#include "functions.c"

int criarNo(FILE* file_indice, char* chave, Cabecalho* reg_cabecalho);
void liberarNo(Node *no);
void split(FILE* file_indice, char* newkey, int newkeyChildRRN, Node *page, char* promoKey, int *promoChild, Node *newpage, Cabecalho* reg_cabecalho);
int insert(int currentRRN, char key[], int *promoChild, char* promoKey, Cabecalho* reg_cabecalho, FILE* file_dados, FILE* file_indice); // lembrar de colocar o arquivo como parametro
void imprimirArvoreB(Node *raiz, int nivel);
void setNode(Node* node);
int getroot(FILE* file_indice);
int getpage(FILE* file_indice);
void set_cabecalho(FILE* file_indice, Cabecalho* reg_cabecalho, char status, int noRaiz, int RRNproxNo);
int search(char* key, Node *page, int *pos);

int criarNo(FILE* file_indice, char* chave, Cabecalho* reg_cabecalho){

    //Node* novoNo = (Node*) malloc(sizeof(Node));
    Node novoNo;

    novoNo.RRNdoNo = getpage(file_indice); // %%%%%%#@@@@@@@ TALVEZ ISSO ESTEJA MUITO ERRADO
    setNode(&novoNo);
    strcpy(novoNo.chaves[0].chave, chave);

    novoNo.nroChavesNo = 1; // começa com 1 mesmo? Será que não é zero?
    novoNo.alturaNo = 1;
    //novoNo.RRNdoNo = 0; // Definindo o valor inicial do RRN do nó ?????????????????????????? getpage() ou isso mesmo?

    fseek(file_indice, 205, SEEK_SET); // &#$¨#!$&¨! CONSIDERAR ERROS RELACIONADOS A "\0" no lixo do cabecalho, ir para 205 ou 206, etc
    fwrite(&novoNo.nroChavesNo, 4, 1, file_indice);
    fwrite(&novoNo.alturaNo, 4, 1, file_indice);
    fwrite(&novoNo.RRNdoNo, 4, 1, file_indice);
    for (int i = 0; i < TAM_ORDEM; i++){
        fwrite(&novoNo.filhos[i], 4, TAM_ORDEM,file_indice);
        if (i < TAM_ORDEM - 1)
            fwrite(&novoNo.chaves[i].chave, 1, 55, file_indice);
    }
    
    set_cabecalho(file_indice, reg_cabecalho, '0', novoNo.RRNdoNo, novoNo.RRNdoNo+1);

    return novoNo.RRNdoNo;
}

void set_cabecalho(FILE* file_indice, Cabecalho* reg_cabecalho, char status, int noRaiz, int RRNproxNo){
    reg_cabecalho->status = status;
    reg_cabecalho->noRaiz = noRaiz;
    reg_cabecalho->RRNproxNo = RRNproxNo;

    char lixo_cabecalho[196];
    memset(lixo_cabecalho, '$', 196);
    lixo_cabecalho[195] = '\0';

    fseek(file_indice, 0, SEEK_SET);

    //  ESCRITA DO CABECALHO NO ARQUIVO E INDICE
    fwrite(&reg_cabecalho->status, sizeof(char), 1, file_indice);
    fwrite(&reg_cabecalho->noRaiz, sizeof(int), 1, file_indice);
    fwrite(&reg_cabecalho->RRNproxNo, sizeof(int), 1, file_indice);
    fwrite(lixo_cabecalho, 1, 196, file_indice); // AQUI TEM QUE TER &?
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

//void driver(Node** root, FILE* file_dados, FILE* file_indice){
void driver(int* root, FILE* file_dados, FILE* file_indice){

    Cabecalho reg_cabecalho;

    int superroot; // ???????
    char* promoKey;
    int promoRRN;
    char* key;

    if (file_indice != NULL){ // (SE O ARQUIVO EXISTE)
        printf("O arquivo existe.\n");
        //fclose(file_indice);
        *root = getroot(file_indice);
    }
    else{
        // CRIACAO DO ARQUIVO BINARIO DE INDICE
        printf("Arquivo inexistente. Criando arquivo...");
        if ((file_indice = fopen("arvB.bin", "wb+")) != NULL){
            printf("\nArquivo criado.");

            char chave[55];

            // INSERINDO A CHAVE DO RRN 0 DO ARQUIVO BIN DE DADOS NA RAIZ DA ARVORE
            // %%%### @@@@@@@@@@@@@PREENCHER OS TRES ESPAÇOS DE CHAVE OU SOMENTE UM????
            
            strcpy(chave, funcionalidade4("binary.bin", 0).nomeTecnologiaOrigem);
            strcat(chave, funcionalidade4("binary.bin",0).nomeTecnologiaDestino);
            //strcpy(((root)->chaves[0]).chave, chave); // ####@@&!&&@& ISSO ESTÁ COMENTADO PORQUE VOCE AGR N USA MAIS Node root
            //(root)->chaves[0].RRN = 0; //                      "
            //(root)->nroChavesNo++; //             "

            *root = criarNo(file_indice, chave, &reg_cabecalho);

        }
        else{
            printf("\nErro ao criar o arquivo");
            return;
        }
    }

    //set_cabecalho(file_indice, &reg_cabecalho, '0', -1, 0); // status = '0', noRaiz = -1, RRNproxNo = 0


    //RRNROOT = reg_cabecalho.RRNproxNo;

    
    //--------------------------------------------

    // INSERINDO TODAS AS OUTRAS CHAVES NA ARVORE
    int counter = 0;
    
    // O PROGRAMA ESTÁ LERDO POR CAUSA DESSA PARTE ABAIXO
    while (funcionalidade4("binary.bin", counter).nomeTecnologiaOrigem != NULL){
        counter++;
        //int promoChild = 0;
        // *************verificar se essa função insert está sendo executada mesmo sendo somente sendo utilizada pelo if()
//ininsert(int currentRRN, char key[], int *promoChild, char* promoKey, Cabecalho* reg_cabecalho, FILE* file_dados, FILE* file_indice); // lembrar de colocar o arquivo como parametro
        if (insert(*root, key, &promoRRN, promoKey, &reg_cabecalho, file_dados, file_indice) == 1){ // SE INSERT RETORNAR PROMOTION
          //  reg_cabecalho.RRNproxNo++;
            *root = criarNo(file_indice, key, &reg_cabecalho);
        }
    }
    
    //free(empty_key);

} // >>>>>>>>>>>>>>>>>>>> FIM DA FUNCAO DRIVER

int ins_page(char* key, int child, Node* page){
    int i;

    for (i = page->nroChavesNo; strcmp(key, page->chaves[i - 1].chave) < 0  && i > 0; i--){
        page->chaves[i] = page->chaves[i-1];
        page->filhos[i+1] = child;
    }
}

int insert(int currentRRN, char key[], int *promoChild, 
            char* promoKey, Cabecalho* reg_cabecalho, FILE* file_dados, FILE* file_indice){ // lembrar de colocar o arquivo como parametro

    // PROMOTION = 1
    // NO PROMOTION = 0
    // ERROR = -1

    Node page, newpage; // precisa ser ponteiro? Precisa fazer alocação dinamica?
    int PBRRN, found, promovido;
    
    //char PBkey[55];
    char* PBkey = (char*)malloc(55*sizeof(char));
    memset(PBkey, '$', 55);
    PBkey[55] = '\0';
    
    //int pos = 0;
    int pos;

    if (currentRRN == -1){
        strcpy(promoKey, key);
        *promoChild = -1;
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
        //int busca(int RRN, char* key, int foundRRN, int foundPos, FILE* file_indice){
        //found = busca(currentRRN, key, &page.RRNdoNo, &pos, file_indice); // tem certeza que os parametros na entrada tao certos?
        found = search(key, &page, &pos);
        
        if (!found){
            return 0;
        }

        int value = insert(page.filhos[pos], key, &PBRRN, PBkey, reg_cabecalho, file_dados, file_indice); // definiu o PBRRN anteriormente??

        if (value == 0 || value == -1){
            return 0;
            // return value; ??? ***************@@@@@@&128
        }
        else{
            if (page.nroChavesNo < TAM_ORDEM - 1){
                ins_page(PBkey, PBRRN, &page);
                // ********* btwrite?
                return 0;
            }
            else{
                split(file_indice, PBkey, PBRRN, &page, promoKey, promoChild, &newpage, reg_cabecalho);
                fseek(file_indice, (currentRRN+1)*205, SEEK_SET); // fseek está certo mesmo?
                fwrite(&page.nroChavesNo, 4, 1, file_indice);
                fwrite(&page.alturaNo, 4, 1, file_indice);
                fwrite(&page.RRNdoNo, 4, 1, file_indice);
                // Pi, Ci, PRi
                for (int w = 0; w < TAM_ORDEM; w++){
                    fwrite(&page.filhos[w], 4, TAM_ORDEM, file_indice);
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
                    fwrite(&newpage.filhos[w], 4, TAM_ORDEM, file_indice);
                    if (w < TAM_ORDEM - 1){
                        fwrite(&newpage.chaves[w].chave, 1, 55, file_indice);
                        fwrite(&newpage.chaves[w].RRN, 4, 1, file_indice);    
                    }
                }

                return 1;
            }
        }
    } // FIM DO ELSE MAIS EXTERNO A TODOS (currentRRN != -1)

    free(PBkey);
}


void liberarNo(Node *no){
	free(no);
}

int getroot(FILE* file_indice){
    // buffer, sizeof, numero, ponteiro
    int root;
    int fseek(); // @@@@&@&@&@&@&@&@&@&&@ ISSO É MESMO NECESSÁRIO???

    fseek(file_indice, 1, SEEK_SET);
    if (fread(&root, 4, 1, file_indice) == 0){
        printf("Erro ao buscar o RRN da raiz");
    }
    return root;
}

int getpage(FILE* file_indice){
    long lseek(), addr;
    // ***********************  SERÁ QUE É PRECISO CONSIDERAR O CABEÇALHO NESSE LSEEK???
    addr = lseek(file_indice, 0L, 2) - 2L;
    return ((int) addr / sizeof(Node));
}

void setNode(Node* node){

    // CRIANDO CHAVES VAZIAS
    //char empty_key[55];
    char* empty_key = (char*) malloc(55*sizeof(char));
    memset(empty_key, '$', 55);
    empty_key[54] = '\0';
    
    int j;
    for (int j = 0; j < TAM_ORDEM - 1; j++){
        strcpy(node->chaves[j].chave, empty_key);
        node->filhos[j] = -1;
    }
    node->filhos[TAM_ORDEM - 1] = -1;

}

void split(FILE* file_indice, char* newkey, int newkeyChildRRN, Node *page, char* promoKey, int *promoChild, Node *newpage, Cabecalho* reg_cabecalho){
    
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
	
    tmpPage.alturaNo = page->alturaNo;

    int i;
	for (i = 0; i < TAM_ORDEM - 1; i++){
        strcpy(tmpPage.chaves[i].chave, page->chaves[i].chave);
        tmpPage.filhos[i] = page->filhos[i];
	}
    tmpPage.filhos[i] = page->filhos[i];
	//tmpPage.nroChavesNo = TAM_ORDEM - 1;

    for (i = TAM_ORDEM - 1; strcmp(newkey, tmpPage.chaves[i - 1].chave) < 0 && i > 0; i--){
        strcpy(tmpPage.chaves[i].chave, tmpPage.chaves[i - 1].chave);
        tmpPage.filhos[i + 1] = tmpPage.filhos[i];
    }

    strcpy(tmpPage.chaves[i].chave, newkey);
    tmpPage.filhos[i+1] = newkeyChildRRN;

    //*promoChild = getpage(file_indice);
    *promoChild = conta_registros(file_indice);
    setNode(newpage);

    for (i=0; i < TAM_ORDEM/2 - 1; i++) {
        strcpy(page->chaves[i].chave, tmpPage.chaves[i].chave);
        page->filhos[i] = tmpPage.filhos[i];
        strcpy(newpage->chaves[i].chave, tmpPage.chaves[i+TAM_ORDEM/2].chave);
        newpage->filhos[i] = tmpPage.filhos[i+TAM_ORDEM/2];
        strcpy(page->chaves[i+TAM_ORDEM/2 - 1].chave, "$\0");
        page->filhos[i+TAM_ORDEM/2] = -1;
    }

    page->filhos[TAM_ORDEM/2 - 1] = tmpPage.filhos[TAM_ORDEM/2 -1];
    newpage->filhos[TAM_ORDEM/2 - 1] = tmpPage.filhos[i + TAM_ORDEM/2];
    newpage->nroChavesNo = (TAM_ORDEM - 1) - TAM_ORDEM/2 - 1;
    page->nroChavesNo = TAM_ORDEM/2 - 1;
    strcpy(promoKey, tmpPage.chaves[TAM_ORDEM/2 - 1].chave); // ???? verificar essa linha
    
}

int busca(int RRN, char* key, int *foundRRN, int *foundPos, FILE* file_indice){

    // 1 = FOUND; -1 = NOT FOUND

    Node page;

    if (RRN = -1){
        printf("pagina não encontrada");
        return -1;
    }
    
    //fseek(file_indice, 1, SEEK_SET);

    //fread(&RRN, 4, 1, file_indice);
    fseek(file_indice, (RRN+1)*205, SEEK_SET);

    // LENDO OS BAGULHO DO NÓ RAIZ E ATRIBUINDO-OS A PAGE
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
            *foundRRN = RRN;
            *foundPos = i;
            return 1;
        }
        else{
            for (int j = 0; j < TAM_ORDEM - 1; j++){
                if (strcmp(key, page.chaves[j].chave) > 0){
                    if (j == TAM_ORDEM - 2){
                        return busca(page.filhos[j], key, foundRRN, foundPos, file_indice);
                    }
                    else{
                        j++;
                    }
                }
                else{
                    return busca(page.filhos[j - 1], key, foundRRN, foundPos, file_indice);
                }
            } // fim do for
        }
    } // fim do primeiro for
    
}

int search(char* key, Node *page, int *pos){
    int i;
    for (i = 0; i < page->nroChavesNo && strcmp(key, page->chaves[i].chave); i++)
        ;
    *pos = i;
    //if (*pos < page->nroChavesNo && key == page->chaves[*pos].chave)
    if (*pos < page->nroChavesNo && strcmp(key, page->chaves[*pos].chave) == 0)
        return 1; 
    else
        return 0;
}


int main(){

    FILE* file_dados = fopen("binary.bin", "rb");
    FILE* file_indice = fopen("arvB.bin", "rb+");

    //Node root;
    int root;

    //Node* root = criarNo(); // CRIANDO A RAIZ DA ARVORE (root = malloc(sizeof(Node)))
    //driver(&root, file_dados, file_indice);
    printf("teste inicio\n");
    driver(&root,file_dados, file_indice);
    printf("\nteste final");

    //free(root);
    fclose(file_indice);
    fclose(file_dados);

	return 0;
}
