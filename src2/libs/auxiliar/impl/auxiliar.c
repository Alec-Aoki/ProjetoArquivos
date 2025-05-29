#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../auxiliar.h"

int str_to_int(char *str)
{
    if (str == NULL)
        return -1; // Erro

    if (strcmp(str, "") == 0)
        return -1; // Erro

    return atoi(str);
}

float str_to_float(char *str)
{
    if (str == NULL)
        return -1; // Erro

    if (strcmp(str, "") == 0)
        return -1; // Erro

    return atof(str);
}

char *formata_string_registro(char *string, char *id)
{
    // Aloca memória para a string
    char *strTemp = (char *)malloc(sizeof(char) * 256);
    if (strTemp == NULL)
        return string; // Erro de alocação de memória

    if (string == NULL || strcmp(string, "") == 0)
    {
        strcpy(strTemp, "NADA CONSTA");
    }
    else
    {
        // Construção segura da string
        strTemp[0] = '\0'; // Inicializa o buffer
        strcat(strTemp, id);
        strcat(strTemp, string);
        strcat(strTemp, "|");
    }

    return strTemp; // Retorna a string formatada
}

char *separa_campo(char **pontStr, int id)
{
    if (**pontStr != (id + '0'))
    {
        return strdup("NADA CONSTA");
    }
    // Recebe um ponteiro para a primeira ocorrecia do caractere dado (Fim da string)
    char *lugarDelimitador = strchr(*pontStr, '|');
    // Aponta para o inicio pulando o ID
    char *inicio = *pontStr + 1;

    // Calcula do tamanho da string
    int tamStr = lugarDelimitador - inicio;

    // Aloca memória para string e verifica se ocorreu corretamente
    char *campo = (char *)malloc(tamStr + 1);
    if (campo == NULL)
        return NULL;

    // Copia a string e adiciona o caractere terminador
    memcpy(campo, inicio, tamStr);
    campo[tamStr] = '\0';

    // Avança o ponteiro para o próximo campo
    *pontStr = lugarDelimitador + 1;

    // retorna a string sem id nem delimitador
    return campo;
}

/*FUNÇÃO FORNECIDA PARA CORREÇÃO*/
void binarioNaTela(char *nomeArquivoBinario)
{ /* Você não precisa entender o código dessa função. */

    /* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
     *  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb")))
    {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++)
    {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}