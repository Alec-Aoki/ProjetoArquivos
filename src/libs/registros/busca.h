/*
Lida com funções de busca de um dado
*/

#ifndef BUSCA_H
#define BUSCA_H

typedef struct busca_ BUSCA;

/* busca_criar():
Aloca memória para uma struct do tipo busca e a inicializa
Retorna: ponteiro para struct (NULL se mal sucedido)
*/
BUSCA *busca_criar();

/* busca_apagar():
Desaloca uma struct do tipo busca
Parâmetro: ponteiro duplo para struct do tipo busca
*/
void busca_apagar(BUSCA **busca);

/* busca_get_quaisCampos():
Retorna o valor de um campo do vetor quaisCampos
de uma struct busca
Parâmetros: ponteiro para struct, inteiro (campo)
Retorna: inteiro
*/
int busca_get_quaisCampos(BUSCA *busca, int campo);

/* busca_ler():
Lê um input do usuário e preenche uma struct busca.
Se a struct não existir, cria uma
Parâmetros: ponteiro para struct busca
Retorna: ponteiro para struct busca (NULL se mal sucedido)
*/
BUSCA *busca_ler(BUSCA *busca);

/* busca_comparar():
Compara uma struct dado com os parâmetros de uma struct busca
Parâmetros: ponteiro para struct busca, ponteiro para struct dado
Retorna: booleano (true se forem iguais, falso se não)
*/
bool busca_comparar(BUSCA *busca, DADO *dado);

/*busca_atualizar_dado():
Atualiza uma struct dado pré-existente com os campos de uma struct busca
Parâmetros: ponteiro para struct busca, ponteiro para struct dado
Retorna: ponteiro para dado
*/
DADO *busca_atualizar_dado(BUSCA *busca, DADO *dado);

/*print_busca():
Imprime todos os campos de uma struct busca
Parâmetro: ponteiro para busca
*/
void print_busca(BUSCA *busca);

#endif