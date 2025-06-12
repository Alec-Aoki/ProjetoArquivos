/*
Contém funções auxilires que não se
encaixam em nenhuma outra biblioteca
*/

#ifndef AUXILIAR_H
#define AUXILIAR_H

#define TAM_MAX_STR 256

/* mensagem_erro()
Imprime a mensagem de erro: "Falha no processamento do arquivo."
*/
void mensagem_erro();

/* mensagem_regInexistente()
Imprime a mensagem: "Registro inexistente."
*/
void mensagem_regInexistente();

/* str_to_int():
A função converte uma string numérica para um inteiro
Parâmetros: ponteiro para uma string
Retorno : o inteiro correspondente, ou -1 caso a string seja nula
*/
int str_to_int(char *str);

/* str_to_float():
A função converte uma string numérica para um float
Parâmetros: ponteiro para uma string
Retorno : o float correspondente, ou -1 caso a string seja nula
*/
float str_to_float(char *str);

/* formata_string_registro():
Aloca dinamicamente memória para uma string e
adiciona delimitadores no inicio e final
Parâmetros: dois ponteiros para string
(um a string em si, a outra o id dessa string)
Retorna: ponteiro para string formatada
*/
char *formata_string_registro(char *string, char *id);

/* separa_campo():
A partir de um ponteiro que aponta para o inicio de uma
string do registro, separa os campos dela
Parâmetro: ponteiro para ponteiro de char
Retorna: uma string
*/
char *separa_campo(char **pontStr, int id);

/* ler_nome_arquivo()
Lê uma string do usuário e atribui à string passada como parâmetro
Parâmetro: ponteiro para char (string)
*/
void ler_nome_arquivo(char *nomeArquivo);

/* ler_entrada_insert()
Lê a entrada do usuário para inserir um novo registro
Parâmetro: nenhum
Retorna: um ponteiro para um vetor de strings
(cada string representa um campo do registro)
*/
char **ler_entrada_insert();

/* apaga_entrada():
Desaloca a memória alocada para a entrada e define o ponteiro para NULL
Parâmetro: ponteiro para ponteiro de ponteiro de char (entrada)
Retorna: nenhum
*/
void apaga_entrada(char ***entrada);

/* tira_aspas()
Remove aspas do início e do fim de uma string, se existirem
Parâmetro: ponteiro para char (string)
Retorna: ponteiro para a nova string sem aspas ou
a string original se não tiver aspas
*/
char *tira_aspas(char *str);

/*FUNÇÃO FORNECIDA PARA LEITURA DE STRINGS*/
void scan_quote_string(char *str); 

/*FUNÇÃO FORNECIDA PARA CORREÇÃO*/
void binarioNaTela(char *nomeArquivoBinario);

#endif