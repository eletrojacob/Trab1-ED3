#include "header.h"

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

Registro funcionalidade4(char* input_string, int n){
	FILE *arq;

	arq = fopen(input_string, "rb");

	Registro* registros = malloc(TAM*sizeof(Registro));

	leitura_binario(registros, arq); // lendo o arquivo binario e configurando a lista de registros

	int i = 0;
	while(registros[i].nomeTecnologiaOrigem != NULL){ // contando a quantidade de registros
		i++;
	}

	if (i > n){ // verifica se o numero fornecido eh menor que a quantidade de registros
		return registros[n];
	}
	else{
        Registro reg_nulo;
        reg_nulo.nomeTecnologiaOrigem = NULL;
		return reg_nulo;
	}

}