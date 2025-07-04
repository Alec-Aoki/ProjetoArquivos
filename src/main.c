#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

// #include "./libs/arquivos/funcionalidades.h"

#include "./libs/arquivos/arvB.h"

// Função para testar criação e manipulação do header
int teste_header()
{
    printf("\n=== TESTE DO HEADER ===\n");

    // Teste 1: Criação do header
    printf("1. Testando criação do header...\n");
    HEADER_ARVB *header = ArvB_header_criar();
    if (header == NULL)
    {
        printf("ERRO: Falha ao criar header\n");
        return 0;
    }
    printf("   ✓ Header criado com sucesso\n");

    // Teste 2: Verificação dos valores iniciais
    printf("2. Verificando valores iniciais...\n");
    int noRaiz = ArvB_header_get_int(header, 1);
    int proxRRN = ArvB_header_get_int(header, 2);
    int nroNos = ArvB_header_get_int(header, 3);

    printf("   noRaiz: %d (esperado: -1)\n", noRaiz);
    printf("   proxRRN: %d (esperado: 0)\n", proxRRN);
    printf("   nroNos: %d (esperado: 0)\n", nroNos);

    if (noRaiz != -1 || proxRRN != 0 || nroNos != 0)
    {
        printf("ERRO: Valores iniciais incorretos\n");
        ArvB_header_apagar(&header);
        return 0;
    }
    printf("   ✓ Valores iniciais corretos\n");

    // Teste 3: Modificação do header
    printf("3. Testando modificação do header...\n");
    header = ArvB_header_set(header, 1, 100, 5, 3);
    if (header == NULL)
    {
        printf("ERRO: Falha ao modificar header\n");
        return 0;
    }

    noRaiz = ArvB_header_get_int(header, 1);
    proxRRN = ArvB_header_get_int(header, 2);
    nroNos = ArvB_header_get_int(header, 3);

    printf("   noRaiz: %d (esperado: 100)\n", noRaiz);
    printf("   proxRRN: %d (esperado: 5)\n", proxRRN);
    printf("   nroNos: %d (esperado: 3)\n", nroNos);

    if (noRaiz != 100 || proxRRN != 5 || nroNos != 3)
    {
        printf("ERRO: Modificação do header falhou\n");
        ArvB_header_apagar(&header);
        return 0;
    }
    printf("   ✓ Modificação do header bem-sucedida\n");

    // Teste 4: Limpeza
    printf("4. Testando limpeza do header...\n");
    ArvB_header_apagar(&header);
    if (header != NULL)
    {
        printf("ERRO: Header não foi definido como NULL após limpeza\n");
        return 0;
    }
    printf("   ✓ Header limpo corretamente\n");

    return 1;
}

// Função para testar criação e manipulação de nós
int teste_no()
{
    printf("\n=== TESTE DOS NÓS ===\n");

    // Teste 1: Criação do nó
    printf("1. Testando criação do nó...\n");
    NO *no = ArvB_no_criar();
    if (no == NULL)
    {
        printf("ERRO: Falha ao criar nó\n");
        return 0;
    }
    printf("   ✓ Nó criado com sucesso\n");

    // Teste 2: Verificação dos valores iniciais
    printf("2. Verificando valores iniciais do nó...\n");
    int byteOffset = ArvB_no_get_int(no, 1);
    int tipoNo = ArvB_no_get_int(no, 2);
    int quantChaves = ArvB_no_get_int(no, 3);

    printf("   byteOffset: %d (esperado: -1)\n", byteOffset);
    printf("   tipoNo: %d (esperado: -1)\n", tipoNo);
    printf("   quantChaves: %d (esperado: 0)\n", quantChaves);

    if (byteOffset != -1 || tipoNo != -1 || quantChaves != 0)
    {
        printf("ERRO: Valores iniciais do nó incorretos\n");
        ArvB_no_apagar(&no);
        return 0;
    }
    printf("   ✓ Valores iniciais do nó corretos\n");

    // Teste 3: Modificação do nó
    printf("3. Testando modificação do nó...\n");
    int chaves[2] = {10, 20};
    int byteOffsetDados[2] = {100, 200};
    int byteOffsetDesc[3] = {50, 150, 250};

    no = ArvB_no_set(no, 300, chaves, byteOffsetDados, byteOffsetDesc, 0, 2);
    if (no == NULL)
    {
        printf("ERRO: Falha ao modificar nó\n");
        return 0;
    }

    byteOffset = ArvB_no_get_int(no, 1);
    tipoNo = ArvB_no_get_int(no, 2);
    quantChaves = ArvB_no_get_int(no, 3);

    printf("   byteOffset: %d (esperado: 300)\n", byteOffset);
    printf("   tipoNo: %d (esperado: 0)\n", tipoNo);
    printf("   quantChaves: %d (esperado: 2)\n", quantChaves);

    if (byteOffset != 300 || tipoNo != 0 || quantChaves != 2)
    {
        printf("ERRO: Modificação do nó falhou\n");
        ArvB_no_apagar(&no);
        return 0;
    }
    printf("   ✓ Modificação do nó bem-sucedida\n");

    // Teste 4: Limpeza
    printf("4. Testando limpeza do nó...\n");
    ArvB_no_apagar(&no);
    if (no != NULL)
    {
        printf("ERRO: Nó não foi definido como NULL após limpeza\n");
        return 0;
    }
    printf("   ✓ Nó limpo corretamente\n");

    return 1;
}

// Função para testar operações de arquivo
int teste_arquivo()
{
    printf("\n=== TESTE DE OPERAÇÕES DE ARQUIVO ===\n");

    // Teste 1: Criação do arquivo
    printf("1. Testando criação do arquivo...\n");
    FILE *arquivo = fopen("teste_arvb.bin", "wb+");
    if (arquivo == NULL)
    {
        printf("ERRO: Falha ao criar arquivo\n");
        return 0;
    }
    printf("   ✓ Arquivo criado com sucesso\n");

    // Teste 2: Escrita e leitura do header
    printf("2. Testando escrita/leitura do header...\n");
    HEADER_ARVB *header = ArvB_header_criar();
    header = ArvB_header_set(header, 1, 100, 5, 3);

    ArvB_header_escrever(arquivo, header);

    HEADER_ARVB *headerLido = ArvB_header_ler(arquivo, NULL);
    if (headerLido == NULL)
    {
        printf("ERRO: Falha ao ler header\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    int noRaiz = ArvB_header_get_int(headerLido, 1);
    int proxRRN = ArvB_header_get_int(headerLido, 2);
    int nroNos = ArvB_header_get_int(headerLido, 3);

    printf("   noRaiz lido: %d (esperado: 100)\n", noRaiz);
    printf("   proxRRN lido: %d (esperado: 5)\n", proxRRN);
    printf("   nroNos lido: %d (esperado: 3)\n", nroNos);

    if (noRaiz != 100 || proxRRN != 5 || nroNos != 3)
    {
        printf("ERRO: Dados do header não correspondem após leitura\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        ArvB_header_apagar(&headerLido);
        return 0;
    }
    printf("   ✓ Header escrito/lido corretamente\n");

    // Teste 3: Escrita e leitura de nó
    printf("3. Testando escrita/leitura de nó...\n");
    NO *no = ArvB_no_criar();
    int chaves[2] = {10, 20};
    int byteOffsetDados[2] = {100, 200};
    int byteOffsetDesc[3] = {50, 150, 250};

    no = ArvB_no_set(no, TAM_HEADER_ARVB, chaves, byteOffsetDados, byteOffsetDesc, 0, 2);

    ArvB_no_escrever(arquivo, no);

    NO *noLido = ArvB_no_ler(arquivo, TAM_HEADER_ARVB);
    if (noLido == NULL)
    {
        printf("ERRO: Falha ao ler nó\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        ArvB_header_apagar(&headerLido);
        ArvB_no_apagar(&no);
        return 0;
    }

    int byteOffset = ArvB_no_get_int(noLido, 1);
    int tipoNo = ArvB_no_get_int(noLido, 2);
    int quantChaves = ArvB_no_get_int(noLido, 3);

    printf("   byteOffset lido: %d (esperado: %d)\n", byteOffset, TAM_HEADER_ARVB);
    printf("   tipoNo lido: %d (esperado: 0)\n", tipoNo);
    printf("   quantChaves lido: %d (esperado: 2)\n", quantChaves);

    if (byteOffset != TAM_HEADER_ARVB || tipoNo != 0 || quantChaves != 2)
    {
        printf("ERRO: Dados do nó não correspondem após leitura\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        ArvB_header_apagar(&headerLido);
        ArvB_no_apagar(&no);
        ArvB_no_apagar(&noLido);
        return 0;
    }
    printf("   ✓ Nó escrito/lido corretamente\n");

    // Limpeza
    fclose(arquivo);
    ArvB_header_apagar(&header);
    ArvB_header_apagar(&headerLido);
    ArvB_no_apagar(&no);
    ArvB_no_apagar(&noLido);

    // Remover arquivo de teste
    remove("teste_arvb.bin");

    return 1;
}

// Função para testar inserção na árvore B
int teste_insercao()
{
    printf("\n=== TESTE DE INSERÇÃO ===\n");

    // Teste 1: Primeira inserção (árvore vazia)
    printf("1. Testando primeira inserção (árvore vazia)...\n");
    FILE *arquivo = fopen("teste_insercao.bin", "wb+");
    if (arquivo == NULL)
    {
        printf("ERRO: Falha ao criar arquivo para inserção\n");
        return 0;
    }

    HEADER_ARVB *header = ArvB_header_criar();
    ArvB_header_escrever(arquivo, header);

    // Inserir primeira chave
    ArvB_inserir(arquivo, header, 10, 1000);

    // Verificar se a inserção foi bem-sucedida
    int noRaiz = ArvB_header_get_int(header, 1);
    int nroNos = ArvB_header_get_int(header, 3);

    printf("   noRaiz após inserção: %d (esperado: %d)\n", noRaiz, TAM_HEADER_ARVB);
    printf("   nroNos após inserção: %d (esperado: 1)\n", nroNos);

    if (noRaiz != TAM_HEADER_ARVB || nroNos != 1)
    {
        printf("ERRO: Primeira inserção falhou\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }
    printf("   ✓ Primeira inserção bem-sucedida\n");

    // Teste 2: Inserção adicional (nó folha com espaço)
    printf("2. Testando inserção adicional (nó folha com espaço)...\n");
    ArvB_inserir(arquivo, header, 5, 500);

    // Verificar se ainda temos apenas um nó
    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após segunda inserção: %d (esperado: 1)\n", nroNos);

    if (nroNos != 1)
    {
        printf("ERRO: Segunda inserção causou split desnecessário\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }
    printf("   ✓ Segunda inserção bem-sucedida\n");

    // Teste 3: Inserção que causa split
    printf("3. Testando inserção que causa split...\n");
    ArvB_inserir(arquivo, header, 15, 1500);

    // Verificar se houve split (devemos ter mais nós)
    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após terceira inserção: %d (esperado: 3)\n", nroNos);

    if (nroNos != 3)
    {
        printf("ERRO: Split não ocorreu corretamente\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }
    printf("   ✓ Split ocorreu corretamente\n");

    // Teste 4: Inserções adicionais
    printf("4. Testando inserções adicionais...\n");
    int chaves[] = {20, 25, 30, 35, 40};
    int byteOffsets[] = {2000, 2500, 3000, 3500, 4000};

    for (int i = 0; i < 5; i++)
    {
        ArvB_inserir(arquivo, header, chaves[i], byteOffsets[i]);
        printf("   Inserida chave %d\n", chaves[i]);
    }

    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após todas as inserções: %d\n", nroNos);
    printf("   ✓ Inserções adicionais concluídas\n");

    // Limpeza
    fclose(arquivo);
    ArvB_header_apagar(&header);
    remove("teste_insercao.bin");

    return 1;
}

// Função para testar busca na árvore B
int teste_busca()
{
    printf("\n=== TESTE DE BUSCA ===\n");

    // Criar árvore com algumas chaves
    printf("1. Criando árvore para teste de busca...\n");
    FILE *arquivo = fopen("teste_busca.bin", "wb+");
    if (arquivo == NULL)
    {
        printf("ERRO: Falha ao criar arquivo para busca\n");
        return 0;
    }

    HEADER_ARVB *header = ArvB_header_criar();
    ArvB_header_escrever(arquivo, header);

    // Inserir algumas chaves
    int chaves[] = {10, 5, 15, 20, 25};
    int byteOffsets[] = {1000, 500, 1500, 2000, 2500};

    for (int i = 0; i < 5; i++)
    {
        ArvB_inserir(arquivo, header, chaves[i], byteOffsets[i]);
        printf("   Inserida chave %d\n", chaves[i]);
    }

    // Teste 2: Buscar chaves existentes
    printf("2. Testando busca de chaves existentes...\n");
    int noRaiz = ArvB_header_get_int(header, 1);

    for (int i = 0; i < 5; i++)
    {
        NO *resultado = ArvB_busca(arquivo, noRaiz, chaves[i]);
        if (resultado == NULL)
        {
            printf("ERRO: Chave %d não encontrada\n", chaves[i]);
            fclose(arquivo);
            ArvB_header_apagar(&header);
            return 0;
        }
        printf("   ✓ Chave %d encontrada\n", chaves[i]);
        ArvB_no_apagar(&resultado);
    }

    // Teste 3: Buscar chaves inexistentes
    printf("3. Testando busca de chaves inexistentes...\n");
    int chavesInexistentes[] = {1, 7, 12, 18, 30};

    for (int i = 0; i < 5; i++)
    {
        NO *resultado = ArvB_busca(arquivo, noRaiz, chavesInexistentes[i]);
        if (resultado != NULL)
        {
            printf("ERRO: Chave %d foi encontrada (não deveria existir)\n", chavesInexistentes[i]);
            ArvB_no_apagar(&resultado);
            fclose(arquivo);
            ArvB_header_apagar(&header);
            return 0;
        }
        printf("   ✓ Chave %d não encontrada (correto)\n", chavesInexistentes[i]);
    }

    // Limpeza
    fclose(arquivo);
    ArvB_header_apagar(&header);
    remove("teste_busca.bin");

    return 1;
}

// Função principal
int main()
{
    printf("=== TESTE COMPLETO DO TAD ÁRVORE B ===\n");

    int sucessos = 0;
    int total = 5;

    // Executar todos os testes
    if (teste_header())
        sucessos++;
    if (teste_no())
        sucessos++;
    if (teste_arquivo())
        sucessos++;
    if (teste_insercao())
        sucessos++;
    if (teste_busca())
        sucessos++;

    // Resultado final
    printf("\n=== RESULTADO FINAL ===\n");
    printf("Testes executados: %d\n", total);
    printf("Testes bem-sucedidos: %d\n", sucessos);
    printf("Testes falharam: %d\n", total - sucessos);

    if (sucessos == total)
    {
        printf("✓ TODOS OS TESTES PASSARAM!\n");
        return 0;
    }
    else
    {
        printf("✗ ALGUNS TESTES FALHARAM!\n");
        return 1;
    }
}