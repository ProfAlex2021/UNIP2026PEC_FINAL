/*
 * arquivo.c - Gravação e leitura de arquivos texto e binário.
 *
 * Todas as funções devolvem:
 *   -1  se o arquivo não pôde ser aberto
 *   >=0 a quantidade de registros gravados/lidos
 */
#include <stdio.h>
#include "arquivo.h"

/*
 * Grava o vetor inteiro de uma só vez.
 * Formato: [int qtd][Pessoa][Pessoa]...
 */
int salvarBinario(const char *nomeArquivo, const Pessoa *vetor, int qtd)
{
    FILE *arq;
    int gravados;

    arq = fopen(nomeArquivo, "wb");
    if (arq == NULL) {
        return -1;
    }

    fwrite(&qtd, sizeof(int), 1, arq);                 /* cabeçalho */
    gravados = fwrite(vetor, sizeof(Pessoa), qtd, arq); /* registros */

    fclose(arq);
    return gravados;
}

/*
 * Lê o arquivo gravado por salvarBinario.
 * O resultado vai para o vetor e para *qtd (variáveis do main).
 */
int carregarBinario(const char *nomeArquivo, Pessoa *vetor, int *qtd)
{
    FILE *arq;
    int total = 0;

    arq = fopen(nomeArquivo, "rb");
    if (arq == NULL) {
        return -1;   /* normal na primeira execução: o arquivo ainda não existe */
    }

    if (fread(&total, sizeof(int), 1, arq) != 1 || total < 0) {
        total = 0;
    }
    if (total > MAX_PESSOAS) {
        total = MAX_PESSOAS;
    }

    *qtd = fread(vetor, sizeof(Pessoa), total, arq);

    fclose(arq);
    return *qtd;
}

/* Exporta para texto, uma pessoa por linha:  nome;idade */
int exportarTexto(const char *nomeArquivo, const Pessoa *vetor, int qtd)
{
    FILE *arq;
    int i;

    arq = fopen(nomeArquivo, "w");
    if (arq == NULL) {
        return -1;
    }

    for (i = 0; i < qtd; i++) {
        fprintf(arq, "%s;%d\n", vetor[i].nome, vetor[i].idade);
    }

    fclose(arq);
    return qtd;
}

/*
 * Lê um arquivo texto no formato  nome;idade  e ACRESCENTA as
 * pessoas ao final do vetor. Linhas fora do formato são ignoradas.
 */
int importarTexto(const char *nomeArquivo, Pessoa *vetor, int *qtd)
{
    FILE *arq;
    char linha[200];
    Pessoa p;
    int importados = 0;

    arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        return -1;
    }

    while (fgets(linha, sizeof(linha), arq) != NULL && *qtd < MAX_PESSOAS) {
        if (sscanf(linha, "%99[^;];%d", p.nome, &p.idade) == 2) {
            adicionarPessoa(vetor, qtd, &p);
            importados++;
        }
    }

    fclose(arq);
    return importados;
}
