#include <stdio.h>

#include "./libs/arquivos/funcionalidades.h"

/*
ALEC CAMPOS AOKI - 15436800
JÕAO RICARDO DE ALMEIDA LUSTOSA - 15463697
*/

int main(void)
{
    char op;

    scanf("%c", &op);

    switch (op)
    {
    case '1': // Funcionalidade 1
        funcionalidade1();
        break;
    case '2': // Funcionalidade 2
        funcionalidade2();
        break;
    case '3': // Funcionalidade 3
        funcionalidade3();
        break;
    }

    return 0;
}