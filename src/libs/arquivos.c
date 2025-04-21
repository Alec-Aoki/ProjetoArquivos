#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "auxiliar.h"
#include "registros.h"
#include "arquivos.h"

/*FUNÇÃO FORNECIDAS PARA CORREÇÃO*/
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

void arquivo_criar(char* nomeArqBin, char* nomeArqCSV){
    // Abre o arquivo .csv para leitura
    FILE* pontArqCSV = fopen(nomeArqCSV, "r");
    if(pontArqCSV == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if(nomeArqBin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    FILE* pontArqBin = fopen(nomeArqBin, "wb");
    if(pontArqBin == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    /* LEITURA DOS CAMPOS DO HEADER */
    char *campos[7]; // Vetor de ponteiros de strings para guardar os campos do header e dos dados
    char buffer[256] = ""; // Buffer para leitura do header e dos campos do .csv

    fseek(pontArqCSV, 0, SEEK_SET); // Posiciona o ponteiro no inicio do arquivo
    
    fread(buffer, 253, 1, pontArqCSV); // Lê a primeira linha do .csv (descrições semânticas do header)
    buffer[253] = '\0'; // Substituindo '\n' por '\0' no buffer, por segurança

    // Vamos utilizar o strtok para separar a string no buffer nas ',' e evitar alocar espaço para as strings do header
    char *tok = strtok(buffer, ",");
    // Guarda cada parte da string em um dos campos do vetor campos
    int i = 0;
    while(tok != NULL && i < 7){
        campos[i] = tok;
        tok = strtok(NULL, ",");
        i++;
    }

    // Criando struct header com os campos semânticos do header do .csv
    HEADER *headerArq = header_criar(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5], campos[6]);
    if(headerArq == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Escrevendo header no arquivo binário
    header_escrever(pontArqBin, headerArq, true); 

    /* LEITURA DOS DADOS */
    // Posiciona o ponteiro no inicio do arquivo
    fseek(pontArqCSV, 0, SEEK_SET);

    // Pega a primeira linha do arquivo e ignora
    fgets(buffer, sizeof(buffer), pontArqCSV);

    // Contadores para atualizar os dados do header
    int quantRegDados = 0;
    long int byteOffset = BYTEOFFSET_HEADER; // Inicializado com o tamanho do header

    while (fgets(buffer, sizeof(buffer), pontArqCSV) != NULL){
        // Identifica o fim da linha lida e substitui o "\n" por um '\0'
        int fimDaLinha = strcspn(buffer, "\n");
        buffer[fimDaLinha] = '\0';

        for (int j = 0; j < 7; j++) {
            campos[j] = "";
        }

        // Variáveis necessárias para a leitura do arquivo
        char *ptr = buffer;     // Ponteiro de char que aponta para o início da string armazenada do buffer
        char *campo;            // Ponteiro de char que receberá as strings entre as vírgulas do csv
        int i = 0;              // Contador do loop para acessar os campos do array

        // Loop que separa os campos da linha lida
        while ((campo = strsep(&ptr, ",")) != NULL) {
            // Guarda a string num vetor de strings se essa não é nula
            if (campo != 0) {
                campos[i] = campo;
                i++;
            }
        }

        // Guarda os dados lidos na struct DADO
        DADO *RegTemp = dado_criar(0, 0, -1, str_to_int(campos[0]), str_to_int(campos[1]), str_to_float(campos[2]), campos[3], campos[4], campos[5], campos[6]);

        quantRegDados++; // Incrementando a quantidade de registros no arquivo

        // Escreve os registros no arquivo binario
        dado_escrever(pontArqBin, RegTemp);
        byteOffset += dado_get_tamanho(RegTemp) + 5; // Atualizando byteOffset
        dado_apagar(&RegTemp);
    }

    /* ATUALIZANDO CAMPOS DO HEADER */
    fseek(pontArqCSV, 0, SEEK_SET);
    // Alterando o status do arquivo antes de fechá-lo
    header_set_status(headerArq, '1');
    header_set_nroRegArq(headerArq, quantRegDados);
    header_set_proxByteOffset(headerArq, byteOffset);

    /* ESCREVENDO HEADER ATUALIZADO NO ARQUIVO */
    header_escrever(pontArqBin, headerArq, false); // Não precisamos escrever os campos semânticos novamente

    /* DESALOCANDO MEMÓRIA E FECHANDO O ARQUIVO */
    header_apagar(&headerArq); // Desalocando struct header
    fclose(pontArqBin); // Fechando o arquivo

    binarioNaTela(nomeArqBin); // Função para o runcodes

    return;
}

void arquivo_imprimir(char* nomeArqBin){
    if(nomeArqBin == NULL){ // Erro com nome do arquivo binário a ser aberto
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE* pontArqBin = fopen(nomeArqBin, "rb"); // Abrindo arquivo binário no modo de leitura
    if(pontArqBin == NULL){ // Erro com a abertura do arquivo
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(pontArqBin, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo

    HEADER* header = header_ler(pontArqBin, NULL); // Leitura do header do arquivo

    int byteOffset = BYTEOFFSET_HEADER; // Inicializado com tamanho do header
    DADO* dado = NULL;

    int quantRegArq = header_get_nroRegArq(header); // Contador para loop de leitura de dados
    if(quantRegArq == 0){
        printf("Registro inexistente.\n");
        return;
    }
    
    // Loop de leitura de dados
    while(quantRegArq > 0){
        dado = dado_ler(pontArqBin, dado, byteOffset);
        // Imprimindo o dado somente se ele não estiver removido
        if(dado_get_removido(dado) != '1'){
            dado_imprimir(header, dado);
            printf("\n");
        }

        byteOffset += dado_get_tamanho(dado) + 5;
        quantRegArq--;
    }

    dado_apagar(&dado);
    header_apagar(&header);
    fclose(pontArqBin);
    return;
}

void arquivo_buscar(char *nomeArqBin, int quantBuscas){
    if(nomeArqBin == NULL) return;

    FILE* pontArqBin = fopen(nomeArqBin, "rb"); // Abrindo arquivo binário no modo de leitura
    if(pontArqBin == NULL){ // Erro com a abertura do arquivo
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(pontArqBin, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo

    HEADER* header = header_ler(pontArqBin, NULL); // Leitura do header do arquivo
    if(header == NULL){ // Erro com a criação do header
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    char buffer[256]; // Buffer para leitura de strings
    int byteOffset = BYTEOFFSET_HEADER; // Byteoffset inicial para leitura dos dados do arquivo
    char *ptr;

    /*Variáveis auxiliares para guardar os valores dos campos a serem pesquisados*/
    int valorInt[2]; // idAttack ou year
    float valorFloat; // financialLoss
    char valorStr[4][20]; // country, attackType, targetIndustry ou defenseMechanism

    int quantRespostas; // Quantidade de dados a serem imprimidos
    int quantCampos; // Quantidade de campos que deverão ser consultados
    int flag; // Indicador de qual campo deve ser buscado
    int j; // Contador para quantCampos
    
    DADO *dado = NULL;
    int quantRegArq; // Variável para guardar a quantidade de registros no arquivo

    /*Loop de buscas*/
    for (int i = 0; i < quantBuscas; i++) {
        quantCampos = 0; // Inicializando a quantidade de campos a serem buscados
        flag = -1; // Inicializando qual campo deve ser buscado
        j = 0; // Inicializando o contador para quantCampos

        scanf("%d", &quantCampos); // Recebendo do usuário quantas respostas (dados) devemos imprimir
        int quaisCampos[quantCampos]; // Vetor para sabermos quais campos buscarmos

        // Lendo string contendo os campos a serem buscados e seus valores
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        ptr = buffer;
       
        /*Lendo qual campo devemos buscar, salvando essa informação no vetor quais campos
        e guardando seus valores nas variáveis auxiliares corretas*/
        char *tok = strsep(&ptr, " ");
        while((tok = strsep(&ptr, " ")) != NULL && j < quantCampos) { // Loop enquanto não chegarmos ao fim da string ou da quantidade de campos
            // Determina qual campo deve ser buscado
            if(strcmp(tok, "idAttack") == 0) flag = 0;
            else if(strcmp(tok, "year") == 0) flag = 1;
            else if(strcmp(tok, "financialLoss") == 0) flag = 2;
            else if(strcmp(tok, "country") == 0) flag = 3;
            else if(strcmp(tok, "attackType") == 0) flag = 4;
            else if(strcmp(tok, "targetIndustry") == 0) flag = 5;
            else if(strcmp(tok, "defenseMechanism") == 0) flag = 6;

            quaisCampos[j] = flag; // Guardando esse campo no vetor
        
            // Avançando o ponteiro na string
            tok = strsep(&ptr, " "); // Tok aponta para o valor do campo que acaba de ser guardado
            if(tok == NULL) break; // Caso tenhamos chegado ao final da string
            
            // Atribuindo esse valor para a variável auxilar correta
            if (flag == 0 || flag == 1) valorInt[flag] = str_to_int(tok);
            else if (flag == 2) valorFloat = str_to_float(tok);
            else if (flag > 2 && flag <= 6) {
                int tamStr = strlen(tok) - 2;
                strncpy(valorStr[flag-3], tok + 1, tamStr);
                valorStr[flag-3][tamStr] = '\0';
            }

            j++; // Avançando o contador para a quantidade de campos e voltando ao início do while
        }
    
        /*Recuperando e imprimindo os dados*/        
        // Preparando para leitura de dados
        byteOffset = BYTEOFFSET_HEADER;
        quantRegArq = header_get_nroRegArq(header);
        bool dadoValido = true; // Flag para verificar se o dado lido obedece à busca requerida
        bool respostaEncontrada = false;

        while(quantRegArq > 0) { // Loop para ler e verificar dados até que o arquivo ou a quantidade de respostas acabe
            dado = dado_ler(pontArqBin, dado, byteOffset); // Lendo dado do arquivo
            if(dado == NULL) break; // Erro

            // Loop para verificar se todos os campos são obedecidos, percorrendo o vetor quaisCampos para isso
            for(int k = 0; k < quantCampos; k++){
                switch(quaisCampos[k]) {
                    // Caso algum dos campos não satisfaça a busca, a flag é settada para false
                    case 0:  // idAttack
                        if(dado_get_idAttacK(dado) != valorInt[0]) dadoValido = false;
                        break;
                    case 1:  // year
                        if(dado_get_year(dado) != valorInt[1]) dadoValido = false;
                        break;
                    case 2:  // financialLoss
                        if(dado_get_finLoss(dado) != valorFloat) dadoValido = false;
                        break;
                    case 3:  // country
                        if(strcmp(dado_get_country(dado), valorStr[0]) != 0) dadoValido = false;
                        break;
                    case 4:  // attackType
                        if(strcmp(dado_get_attackType(dado), valorStr[1]) != 0) dadoValido = false;
                        break;
                    case 5:  // targetIndustry
                        if(strcmp(dado_get_targetIndustry(dado), valorStr[2]) != 0) dadoValido = false;
                        break;
                    case 6:  // defenseMechanism
                        if(strcmp(dado_get_defenseMech(dado), valorStr[3]) != 0) dadoValido = false;
                        break;
                    default:
                        break;
                }
            }
            
            // Imprimindo dado caso ele seja válido
            if(dadoValido){
                respostaEncontrada = true;
                dado_imprimir(header, dado);
                printf("\n");
            }

            dadoValido = true; // Resettando a flag
            
            byteOffset += dado_get_tamanho(dado) + 5; // Atualizando o byteOffset (indo para a leitura do próximo dado)
            quantRegArq--; // Atualizando a quantidade de dados buscada
        }

        if(!respostaEncontrada) printf("Registro inexistente.\n\n"); // Não foi encontrado um dado que obedece os campos de busca
        respostaEncontrada = false; // Resettando flag

        printf("**********\n");
    }

    dado_apagar(&dado);
    header_apagar(&header);
    fclose(pontArqBin);
    return;
}