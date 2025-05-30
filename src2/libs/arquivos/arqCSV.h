/*
Lida com o arquivo .csv
*/

#ifndef ARQCSV_H
#define ARQCSV_H

/*arqCSV_ler_header():
Lê o header de um arquivo .csv e cria uma struct do tipo header com os dados lidos
Parâmetro: ponteiro para arquivo
Retorno: ponteiro para struct do tipo header (NULL se mal sucedido)
*/
HEADER *arqCSV_ler_header(FILE *pontArqCSV);

/*arqCSV_ler_dado():
Lê um dado de um arquivo .csv
Parâmetro: ponteiro para arquivo
Retorno: ponteiro para struct do tipo dado (NULL se mal sucedido)
*/
DADO *arqCSV_ler_dado(FILE *pontArqCSV);

#endif