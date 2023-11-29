#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TAM_STR 100
#define TAM 500

typedef struct{
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
} reg_cabecalho; // Tipo heterogeneo para os atributos do registro de cabecalho

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

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%FUNCOES AUXILIARES%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int calcula_tecs(Registro *regs){
	char** vet_str; // vetor dinamico de strings
	int counter = 0; // contador para o numero de tecnologias
	int i = 0, j = 0;
	int flag;

	vet_str = calloc(TAM,sizeof(char));
	
	while(regs[i+1].nomeTecnologiaOrigem != NULL){
		vet_str[i] = regs[i+1].nomeTecnologiaOrigem; // armazenando os dados de NomeTecnologiaOrigem de cada registro

		j = 0;

		while(strcmp(vet_str[i], vet_str[j])){ // comparacao entre duas strings para verificar repeticoes
			j++;
			if (j == i){
				counter++; // enquanto nao houver repeticoes incrementar o contador do nro de tecnologias
			}
		}
		i++;
		
	} // fim do primeiro while da funcao calcula_tecs

	free(vet_str);
	return counter;

} /////// FIM DA FUNCAO CALCULA_TECS //////////////////////////////////
/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

int calcula_pares(Registro* regs){
	char** vet; // vetor dinamico de strings
	int counter = 0; // contador para o numero de pares de tecnologias
	int i = 0, j = 0; // variaveis de incremento

	vet = calloc(TAM,sizeof(char));
	//vet = malloc(strlen(regs->nomeTecnologiaOrigem)+strlen(regs->nomeTecnologiaDestino));
	//vet = malloc(sizeof(char));

	while (regs[i+1].nomeTecnologiaOrigem != NULL){		
		vet[i] = strcat(regs[i+1].nomeTecnologiaOrigem, regs[i+1].nomeTecnologiaDestino); // concatenando strings
		//sprintf(vet[i],"%s%s", regs[i+1].nomeTecnologiaOrigem, regs[i+1].nomeTecnologiaDestino);

		j = 0;

		while(strcmp(vet[i], vet[j])){ // enquanto nao houver repeticao de pares, incrementar o contador 
			j++;
			if (j == i){
				counter++;
			}
		}
		i++;
	}
	
	free(vet);
	return counter;
} //////////////// FIM DA FUNCAO CALCULA PARES ////////////////////////////////////////////

void leitura_registros(Registro *registros, FILE *arq, reg_cabecalho* cabecalho){
	int i = 0; // contador para as posições da lista de registros

	char* str; // vetor dinamico ao qual serão atribuídas as strings de cada linha do arquivo
	char* aux; //ponteiro auxiliar para captar as virgulas

	str = malloc(TAM*sizeof(char));
	aux = malloc(TAM*sizeof(char));

	while(fgets(str, 100, arq) != NULL){
		
		registros[i].nomeTecnologiaOrigem = (char *)malloc(TAM*sizeof(char));
		registros[i].nomeTecnologiaDestino = (char *)malloc(TAM*sizeof(char));
		registros[i].removido = '0';

		char* aux; // vetor de chars (string) para armazenar os chars de um campo
		char** vec; // vetor dinâmico de strings para armazenar os campos de um registro

		vec = malloc(TAM*sizeof(char));
		
		aux = strtok(str, ","); // variavel auxiliar que armazena o início da string até o primeiro ","

		int j = 0; // contador para o char** vec
		int valor; // valor do tipo inteiro representado pelas strings numericas

		while (aux != NULL){
			vec[j] = aux;
			aux = strtok(NULL, ",");
			j++;
		}
		//fputs(str,stdout);

		//str = strtok(str, ",");
		//registros[i].nomeTecnologiaOrigem = str;
		strcpy(registros[i].nomeTecnologiaOrigem, vec[0]);
		strcpy(registros[i].nomeTecnologiaDestino, vec[3]);
		valor = atoi(vec[1]);
		registros[i].grupo = valor;
		valor = atoi(vec[2]);
		registros[i].popularidade = valor;
		valor = atoi(vec[4]);
		registros[i].peso = valor;

		// printando os campos de cada registro para conferir
		/*printf("%s, ", registros[i].nomeTecnologiaOrigem);
		printf("%d, ", registros[i].grupo);
		printf("%d, ", registros[i].popularidade);
		printf("%s, ", registros[i].nomeTecnologiaDestino);
		printf("%d\n", registros[i].peso);*/

		registros[i].tamanhoTecnologiaOrigem = strlen(registros[i].nomeTecnologiaOrigem);
		registros[i].tamanhoTecnologiaDestino = strlen(registros[i].nomeTecnologiaDestino);
		
		i++;

		//free(registros[i].nomeTecnologiaOrigem);

		//char **new = malloc(sizeof(char));
		//new = registros->nomeTecnologiaOrigem;
	} // >>>>>>>>>>>  fim do while <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	cabecalho->proxRRN = i-1;

	i = 0;

	free(str);
	free(aux);
}

void leitura_binario(Registro *registros, FILE *arq){

    int i = 0;
    char byte;

    fseek(arq, 13, SEEK_SET);

    //while (fread(&registros[i].removido, sizeof(char), 1, arq) == 1) {
    while (fread(&registros[i].removido, sizeof(char), 1, arq) != 0) { 
    	
        fread(&registros[i].grupo, sizeof(int), 1, arq);
        fread(&registros[i].popularidade, sizeof(int), 1, arq);
        fread(&registros[i].peso, sizeof(int), 1, arq);
        fread(&registros[i].tamanhoTecnologiaOrigem, sizeof(int), 1, arq);

        registros[i].nomeTecnologiaOrigem = (char *)malloc(registros[i].tamanhoTecnologiaOrigem*sizeof(char)+1);
        fread(registros[i].nomeTecnologiaOrigem, sizeof(char), registros[i].tamanhoTecnologiaOrigem, arq);


        fread(&registros[i].tamanhoTecnologiaDestino, sizeof(int), 1, arq);

        registros[i].nomeTecnologiaDestino = (char *)malloc(registros[i].tamanhoTecnologiaDestino*sizeof(char));
        fread(registros[i].nomeTecnologiaDestino, sizeof(char), registros[i].tamanhoTecnologiaDestino, arq);

        registros[i].nomeTecnologiaOrigem[registros[i].tamanhoTecnologiaOrigem] = '\0';
        registros[i].nomeTecnologiaDestino[registros[i].tamanhoTecnologiaDestino] = '\0';


        fseek(arq, 76 - 21 - registros[i].tamanhoTecnologiaOrigem - registros[i].tamanhoTecnologiaDestino, SEEK_CUR);
        i++;
    }
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

char* conteudoEntreAspas(const char* input) { // FUNCAO DO ChatGPT
    
    const char* inicio = strchr(input, '"'); // Encontra a primeira aspa
    if (inicio == NULL) {
        return NULL; // Não há aspas
    }

    inicio++; // Avança para o próximo caractere após a primeira aspa

    const char* fim = strchr(inicio, '"'); // Encontra a segunda aspa
    if (fim == NULL) {
        return NULL; // Não há segunda aspa
    }

    // Calcula o tamanho do conteúdo entre as aspas
    size_t tamanho = fim - inicio;

    // Aloca memória para o conteúdo entre as aspas
    char* resultado = (char*)malloc(tamanho + 1);
    if (resultado == NULL) {
        return NULL; // Falha na alocação de memória
    }

    // Copia o conteúdo entre as aspas para o resultado
    strncpy(resultado, inicio, tamanho);
    resultado[tamanho] = '\0'; // Adiciona um caractere nulo no final

    return resultado;
}


void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
} //////////////// FIM DA FUNCAO BINARIO NA TELA ////////////////////////////////////////////////////////

void print_registros(Registro registros){

	if (registros.tamanhoTecnologiaOrigem == 0){
		printf("NULO, ");
	}
	else{
		printf("%s, ", registros.nomeTecnologiaOrigem);
	}

	if (registros.grupo == -1){
		printf("NULO, ");
	}
	else{
		printf("%d, ", registros.grupo);
	}

	if (registros.popularidade == -1){
		printf("NULO, ");
	}
	else{
		printf("%d, ", registros.popularidade);
	}

	if (registros.tamanhoTecnologiaDestino == 0){
		printf("NULO, ");
	}
	else{
		printf("%s, ", registros.nomeTecnologiaDestino);
	}

	if (registros.peso == -1){
		printf("NULO\n");
	}
	else{
		printf("%d\n", registros.peso);
	}
}

void campo_compativel(Registro registro, char* campo, char* input){

	if (!strcmp(campo, "peso")){
		if (atoi(input) == registro.peso){
			print_registros(registro);
		}
	}

	if (!strcmp(campo, "popularidade")){
		if (atoi(input) == registro.popularidade){
			print_registros(registro);
		}
	}

	if (!strcmp(campo, "grupo")){
		if (atoi(input) == registro.grupo){
			print_registros(registro);
		}
	}

	if (!strcmp(campo, "nomeTecnologiaOrigem")){
		//scan_quote_string(input);
		if (!strcmp(conteudoEntreAspas(input), registro.nomeTecnologiaOrigem)){
			print_registros(registro);
		}
	}

	if (!strcmp(campo, "nomeTecnologiaDestino")){
		//scan_quote_string(input);
		if (!strcmp(conteudoEntreAspas(input), registro.nomeTecnologiaDestino)){
			print_registros(registro);
		}
	}

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%FIM DAS FUNCS AUXILIARES%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




//!!!!!!!!!!!!!!!!!!!!!!!!!!!!F U N C A O  1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void funcionalidade1(char* input_string, char* outfile_string){
	FILE *arq;

	arq = fopen(input_string, "r");

	if (!fopen(input_string, "r")){  ///// /????
		fprintf(stderr, "Falha no processamento do arquivo.");
		return;
	}

	reg_cabecalho cabecalho;

	cabecalho.status = '0';
	cabecalho.proxRRN = 0;
	cabecalho.nroTecnologias = 0;
	cabecalho.nroParesTecnologias = 0;

	Registro* registros; // ponteiro para lista de registros do tipo Registro
	Registro* registros_aux; // ponteiro auxiliar para recuperar os dados apos a concatenacao

	registros = malloc(TAM*sizeof(Registro));
	registros_aux = malloc(TAM*sizeof(Registro));

	leitura_registros(registros, arq, &cabecalho); // lendo os registros do arquivo e armazenando-os na estrutura

	cabecalho.nroTecnologias = calcula_tecs(registros)+1; // quantidade de tecnologias diferentes
	cabecalho.nroParesTecnologias = calcula_pares(registros)+1; // quantidade de pares diferentes de tecnologias

	//fclose(arq);

	arq = fopen(input_string, "r");

	free(registros);

	leitura_registros(registros_aux, arq, &cabecalho); // leitura para reiniciar os dados armazenados

	//printf("\n\n%d\n", cabecalho.nroTecnologias);
	//printf("%d\n", cabecalho.nroParesTecnologias);


	//---BINARIO---------------------------------------------------------------------
	//-----------------BINARIO-------------------------------------------------------
	//---------------------------------BINARIO --------------------------------------

	arq = fopen(outfile_string, "wb");

	cabecalho.status = '1'; // definindo o arquivo como consistente
	fwrite(&cabecalho.status, sizeof(cabecalho.status), 1, arq); // escrita no arquivo binario
	//printf("%d\n", cabecalho.proxRRN);
	fwrite(&cabecalho.proxRRN, sizeof(cabecalho.proxRRN), 1, arq);
	fwrite(&cabecalho.nroTecnologias, sizeof(cabecalho.nroTecnologias), 1, arq);
	fwrite(&cabecalho.nroParesTecnologias, sizeof(cabecalho.nroParesTecnologias), 1, arq);

	
	int i = 1;
	char x = '$';
	while(registros_aux[i].nomeTecnologiaOrigem != NULL){ // escrita do conjunto de campos de cada registro no arquivo binario
		fwrite(&registros_aux[i].removido, sizeof(registros_aux[i].removido), 1, arq);
		fwrite(&registros_aux[i].grupo, sizeof(registros_aux[i].grupo), 1, arq);
		fwrite(&registros_aux[i].popularidade, sizeof(registros_aux[i].popularidade), 1, arq);
		fwrite(&registros_aux[i].peso, sizeof(registros_aux[i].peso), 1, arq);
		fwrite(&registros_aux[i].tamanhoTecnologiaOrigem, sizeof(registros_aux[i].tamanhoTecnologiaOrigem), 1, arq);
		fwrite(registros_aux[i].nomeTecnologiaOrigem, sizeof(char) * strlen(registros_aux[i].nomeTecnologiaOrigem), 1, arq);
		fwrite(&registros_aux[i].tamanhoTecnologiaDestino, sizeof(registros_aux[i].tamanhoTecnologiaDestino), 1, arq);
		fwrite(registros_aux[i].nomeTecnologiaDestino, sizeof(char) * strlen(registros_aux[i].nomeTecnologiaDestino), 1, arq);

		//76 - tamanhototal - 21 = diferenca
		// diferenca * $
		// deslocando a cabeca de leitora
		for (int k = 0; k < 76 - 21 - registros_aux[i].tamanhoTecnologiaOrigem - registros_aux[i].tamanhoTecnologiaDestino; k++){
			fwrite(&x, sizeof(char), 1, arq);
		}
		i++;
	}

	fclose(arq);
	free(registros_aux);

	binarioNaTela(outfile_string);

}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!F U N C A O  2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void funcionalidade2(char* input_string){
	FILE* arq;

	Registro* registros; // ponteiro para criacao de uma lista de registros
	registros = malloc(TAM*sizeof(Registro));

	arq = fopen(input_string, "rb");

	if (!fopen(input_string, "r")){
		printf("Falha no processamento do arquivo.");
		return;
	}

	int i = 0;

	leitura_binario(registros, arq); // lendo o arquivo binario e configurando a lista de registros

	while(registros[i].nomeTecnologiaOrigem != NULL){ // mostrar registros ate o fim do arquivo
		print_registros(registros[i]);
		i++;
	}
	free(registros);
	fclose(arq);
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!F U N C A O  3 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void funcionalidade3(char* input_string, char vet_nomes_campos[][21], char** vet_valores_campos, int n){
    FILE* arq;

    Registro* registros; // ponteiro para criacao de uma lista de registros
    registros = malloc(TAM*sizeof(Registro));

    arq = fopen(input_string, "rb");

    leitura_binario(registros, arq); // lendo o arquivo binario e configurando a lista de registros

    int i = 0;
    for (int k = 0; k < n; k++){
        while(registros[i].nomeTecnologiaOrigem != NULL){
            // comparando os campos fornecidos com os campos esperados
            campo_compativel(registros[i], vet_nomes_campos[k], vet_valores_campos[k]);
            i++;
        }
        i = 0;
    }

    free(registros);
    fclose(arq);
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!F U N C A O  4 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void funcionalidade4(char* input_string, int n){
	FILE *arq;

	arq = fopen(input_string, "rb");

	Registro* registros = malloc(TAM*sizeof(Registro));

	leitura_binario(registros, arq); // lendo o arquivo binario e configurando a lista de registros

	int i = 0;
	while(registros[i].nomeTecnologiaOrigem != NULL){ // contando a quantidade de registros
		i++;
	}

	if (i > n){ // verifica se o numero fornecido eh menor que a quantidade de registros
		print_registros(registros[n]);
	}
	else{
		printf("Registro inexistente.\n");
	}

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

int main(){

	int input, n;
	char* input_string = (char*)malloc(sizeof(char)*100); // variavel que apontara o nome a ser fornecido do arquivo

	scanf("%d", &input); // leitura do valor referente a funcionalidade

	scanf("%s", input_string); // leitura do nome do arquivo

	switch(input){ // analisar a opcao de funcionalidade escolhida pelo usuario
	case 1:{ // funcionalidade 1
		char* outfile_string = (char*)malloc(sizeof(char)); // ponteiro para o nome do arquivo de saida
		scanf("%s", outfile_string); // leitura do nome do arquivo de saida
		funcionalidade1(input_string, outfile_string);
		free(outfile_string); // desalocacao de memoria
		break;
	}
	case 2: // funcionalidade 2
		funcionalidade2(input_string);
		break;
	case 3:{ // funcionalidade 3
		scanf("%d", &n); // leitura do numero de campos a serem buscados
		char vet_nomes_campos[n][21]; // vetor para indexar e armazenar os nomes dos campos fornecidos
		char** vet_valores_campos = (char **)malloc(n*sizeof(char)); // lista de strings para armazenar os nomes dos valores de cada campo
		for (int i = 0; i < n; i++) {
		vet_valores_campos[i] = (char *)malloc(100); // Aloca espaço para uma string de até 99 caracteres
	}
		for(int w = 0; w < n; w++){ // leitura dos nomes dos campos e os valores correspondentes
			scanf("%s", vet_nomes_campos[w]);
			scanf("%s", vet_valores_campos[w]);
		}

		funcionalidade3(input_string, vet_nomes_campos, vet_valores_campos,n);
		for (int i = 0; i < n; i++) {
        free(vet_valores_campos[i]);
    }
		free(vet_valores_campos);
		break;
	}
	case 4: // funcionalidade 4
		scanf("%d", &n); // leitura do RRN a ser buscado
		funcionalidade4(input_string, n);
		break;
	default:
		break;
	}

	free(input_string);
	return 0;
}