#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../auxiliar.h"

/* mensagem_erro()
Imprime a mensagem de erro: "Falha no processamento do arquivo."
*/
void mensagem_erro()
{
    printf("Falha no processamento do arquivo.\n");
    return;
}

/* mensagem_regInexistente()
Imprime a mensagem: "Registro inexistente."
*/
void mensagem_regInexistente()
{
    printf("Registro inexistente.\n\n");
    return;
}

/* str_to_int():
A função converte uma string numérica para um inteiro
Parâmetros: ponteiro para uma string
Retorno : o inteiro correspondente, ou -1 caso a string seja nula
*/
int str_to_int(char *str)
{
    if (str == NULL)
        return -1; // Erro

    if (strcmp(str, "") == 0)
        return -1; // Erro

    if (strcmp(str, "NULO") == 0)
        return -1; // Erro

    return atoi(str);
}

/* str_to_float():
A função converte uma string numérica para um float
Parâmetros: ponteiro para uma string
Retorno : o float correspondente, ou -1 caso a string seja nula
*/
float str_to_float(char *str)
{
    if (str == NULL)
        return -1; // Erro

    if (strcmp(str, "") == 0)
        return -1; // Erro

    if (strcmp(str, "NULO") == 0)
        return -1; // Erro

    return atof(str);
}

/* formata_string_registro():
Aloca dinamicamente memória para uma string e adiciona delimitadores no inicio e final
Parâmetros: dois ponteiros para string (um a string em si, a outra o id dessa string)
Retorna: ponteiro para string formatada
*/
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

/* separa_campo():
A partir de um ponteiro que aponta para o inicio de uma string do registro, separa os campos dela
Parâmetro: ponteiro para ponteiro de char
Retorna: uma string
*/
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

/* ler_nome_arquivo()
Lê uma string do usuário e atribui à string passada como parâmetro
Parâmetro: ponteiro para char (string)
*/
void ler_nome_arquivo(char *nomeArquivo)
{
    if (nomeArquivo == NULL)
    {
        mensagem_erro();
        return;
    }

    scanf("%s", nomeArquivo);
    return;
}

/* aloca_array_string()
Aloca memória para um vetor de strings, onde cada string tem tamanho TAM_MAX_STR
Parâmetro: nenhum
Retorna: um ponteiro para o vetor de strings alocado ou NULL se falhar
*/
char ** aloca_array_string ()
{
    char **array = (char **)malloc(sizeof(char *) * 7);
    if (array == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    for (int i = 0; i < 7; i++)
    {
        array[i] = (char *)malloc(sizeof(char) * TAM_MAX_STR);
        if (array[i] == NULL)
        {
            mensagem_erro();
            return NULL;
        }
    }

    return array;
}

/* ler_entrada_insert()
Lê a entrada do usuário para inserir um novo registro
Parâmetro: nenhum
Retorna: um ponteiro para um vetor de strings (cada string representa um campo do registro)
*/
char ** ler_entrada_insert()
{
    //getchar();
    char **entrada = aloca_array_string();
    if (entrada == NULL)
    {
        mensagem_erro();
        return NULL; // Erro ao alocar memória
    }
    int i = 0;
    for (; i < 3; i++)
    {
        scanf(" %s", entrada[i]);
        if (strcmp(entrada[i], "NULO") == 0)
        {
            entrada[i] = NULL; // Define como NULL se for "NULO"
        }
    }
    for (; i < 7; i++)
    {
        scan_quote_string(entrada[i]);
        if (strcmp(entrada[i], "NULO") == 0)
        {
            entrada[i] = NULL;
        }
    }

    return entrada;
}

/* apaga_entrada():
Desaloca a memória alocada para a entrada e define o ponteiro para NULL
Parâmetro: ponteiro para ponteiro de ponteiro de char (entrada)
Retorna: nenhum
*/
void apaga_entrada(char ***entrada)
{
    if (*entrada == NULL)
    {
        return;
    }

    for (int i = 0; i < 7; i++)
    {
        if ((*entrada)[i] != NULL)
        {
            free((*entrada)[i]);
        }
    }

    free(*entrada);
    *entrada = NULL;
}

/* tira_aspas()
Remove aspas do início e do fim de uma string, se existirem
Parâmetro: ponteiro para char (string)
Retorna: ponteiro para a nova string sem aspas ou
a string original se não tiver aspas
*/
char *tira_aspas(char *str)
{
    if (str == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    // Verifica se a string começa e termina com aspas
    size_t len = strlen(str);

    if (len > 1 && str[0] == '"' && str[len - 1] == '"')
    {
        // Aloca memória para a nova string sem aspas
        char *temp = (char *)malloc(len - 1);

        if (temp == NULL)
        {
            mensagem_erro();
            return NULL;
        }

        // Copia a string sem as aspas
        strncpy(temp, str + 1, len - 2);
        temp[len - 2] = '\0'; // Adiciona o terminador nulo
        return temp;
    }

    return str; // Retorna a string original se não tiver aspas
}

/*FUNÇÃO FORNECIDA DE LEITURA DE STRING COM ASPAS*/
void scan_quote_string(char *str) 
{

        /*
        *       Use essa função para ler um campo string delimitado entre aspas (").
        *       Chame ela na hora que for ler tal campo. Por exemplo:
        *
        *       A entrada está da seguinte forma:
        *               nomeDoCampo "MARIA DA SILVA"
        *
        *       Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
        *               scanf("%s", str1); // Vai salvar nomeDoCampo em str1
        *               scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
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