#include <stdio.h>

#include "./libs/arquivos/funcionalidades.h"

/*
ALEC CAMPOS AOKI - 15436800
JÕAO RICARDO DE ALMEIDA LUSTOSA - 15463697
*/

int main(void)
{
    int op;

    scanf("%d", &op);

    switch (op)
    {
    case 1: // Funcionalidade 1
        funcionalidade1();
        break;
    case 2: // Funcionalidade 2
        funcionalidade2();
        break;
    case 3: // Funcionalidade 3
        funcionalidade3();
        break;
    case 4: // Funcionalidade 4
        funcionalidade4();
        break;
    case 5: // Funcionalidade 5
        funcionalidade5();
        break;
    case 6: // Funcionalidade 6
        funcionalidade6();
        break;
    case 7: // Funcionalidade 7
        funcionalidade7();
        break;
    case 8: // Funcionalidade 8
        funcionalidade8();
        break;
    case 10: // Funcionalidade 10
        funcionalidade10();
        break;
    case 11: // Funcionalidade 11
        funcionalidade11();
        break;
    }

    return 0;
}