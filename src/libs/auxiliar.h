#ifndef AUXILIAR_H
    #define AUXILIAR_H

    // Converte uma string numérica para um intero
    int str_to_int (char *str);

    // Converte uma string numérica para um float
    float str_to_float (char *str);

    // Aloca dinamicamente memória para uma string e adiciona delimitadores no inicio e final
    char *formata_string_registro (char *string, char *id);

#endif