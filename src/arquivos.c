//
// Created by alecc on 4/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "arquivos.h"

/*Armazena os campos do header que sofrem alterações*/
struct header_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos
};

/*FUNÇÕES AUXILIARES*/
/*
Cria uma struct do tipo HEADER e a inicializa
Parâmetros: void
Retorna: ponteiro para a struct do tipo HEADER
*/
HEADER* header_criar(void){
    HEADER* novoHeader = (HEADER *) malloc(sizeof(HEADER)); // Alocando dinâmicamente uma struct do tipo HEADER
    if(novoHeader == NULL){
        printf("Erro ao criar struct\n");

        return NULL;
    }

    // Inicializando a struct
    novoHeader->status = '0';
    novoHeader->topo = -1;
    novoHeader->proxByteOffset = 0;
    novoHeader->nroRegArq = 0;
    novoHeader->nroRegRem = 0;

    return novoHeader;
}

/*
Cria um arquivo binário e o inicializa com o header
Parâmetros: ponteiro para uma string (nome do arquivo a ser criado)
Retorna:
    Caso bem-sucedido: true
    Caso contrário: false
*/
bool arquivo_criar(char* nomeArquivo){
    if(nomeArquivo == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");

        return false;
    }

    FILE* pontArq = fopen(nomeArquivo, "wb"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArq == NULL){
        printf("Erro ao criar arquivo\n");

        return false;
    }

    HEADER* headerTemp = header_criar();
    if(headerTemp == NULL){
        printf("Erro ao criar struct\n");

        return false;
    }

    // TODO: finish writing header
    fwrite(&(headerTemp->status), sizeof(char), 1, pontArq);
    fwrite(&(headerTemp->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerTemp->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerTemp->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerTemp->nroRegArq), sizeof(int), 1, pontArq);

    //char* string = (char *) malloc(67 * sizeof(char)); // Alocando dinâmicamente uma string de tamanho máximo de 67 caracteres
    //strcpy(string, "0");
    //fwrite(string, sizeof(char), 1, pontArq);

    free(headerTemp); // Desalocando struct HEADER criada
    fclose(pontArq); // Fechando o arquivo

    return true;
}

/*FUNÇÕES FORNECIDAS*/

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