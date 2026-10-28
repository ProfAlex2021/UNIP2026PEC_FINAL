/*
 * pessoa.c - Operações sobre Pessoa usando ponteiros.
 */
#include <stdio.h>
#include <string.h>
#include "pessoa.h"
#include "entrada.h"

/* Preenche a pessoa apontada por p com dados do teclado */
void lerPessoa(Pessoa *p)
{
    do {
        lerTexto("Nome: ", p->nome, TAM_NOME);
        if (strlen(p->nome) == 0) {
            printf("O nome não pode ficar vazio.\n");
        }
    } while (strlen(p->nome) == 0);

    do {
        p->idade = lerInteiro("Idade: ");
        if (p->idade < 0 || p->idade > 130) {
            printf("Idade inválida (0 a 130).\n");
        }
    } while (p->idade < 0 || p->idade > 130);
}

void exibirCabecalho(void)
{
    printf("%-4s | %-40s | %s\n", "Nº", "NOME", "IDADE");
    printf("-----+------------------------------------------+------\n");
}

/* const: garante que esta função não altera a pessoa */
void exibirPessoa(const Pessoa *p, int numero)
{
    printf("%-4d | %-40s | %d\n", numero, p->nome, p->idade);
}

void listarPessoas(const Pessoa *vetor, int qtd)
{
    int i;

    if (qtd == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
        return;
    }
    exibirCabecalho();
    for (i = 0; i < qtd; i++) {
        exibirPessoa(&vetor[i], i + 1);
    }
    printf("\nTotal: %d pessoa(s)\n", qtd);
}

/*
 * Copia *nova para o final do vetor e incrementa *qtd.
 * Devolve 1 se conseguiu, 0 se o vetor está cheio.
 */
int adicionarPessoa(Pessoa *vetor, int *qtd, const Pessoa *nova)
{
    if (*qtd >= MAX_PESSOAS) {
        return 0;
    }
    vetor[*qtd] = *nova;   /* copia a struct inteira */
    (*qtd)++;              /* parênteses obrigatórios! *qtd++ seria outra coisa */
    return 1;
}

/*
 * Procura, a partir da posição 'inicio', a primeira pessoa cujo
 * nome CONTÉM o trecho informado (strstr devolve um ponteiro ou NULL).
 * Devolve o índice encontrado ou -1.
 */
int buscarPorNome(const Pessoa *vetor, int qtd, const char *trecho, int inicio)
{
    int i;

    for (i = inicio; i < qtd; i++) {
        if (strstr(vetor[i].nome, trecho) != NULL) {
            return i;
        }
    }
    return -1;
}

/*
 * Remove a pessoa da posição 'indice' puxando as seguintes uma casa
 * para trás. Devolve 1 se removeu, 0 se o índice é inválido.
 */
int removerPessoa(Pessoa *vetor, int *qtd, int indice)
{
    int i;

    if (indice < 0 || indice >= *qtd) {
        return 0;
    }
    for (i = indice; i < *qtd - 1; i++) {
        vetor[i] = vetor[i + 1];
    }
    (*qtd)--;
    return 1;
}

/*
 * Devolve um PONTEIRO para a pessoa mais velha do vetor
 * (ou NULL se o vetor estiver vazio). Quem chama pode usar
 * o ponteiro para ler ou até alterar essa pessoa.
 */
Pessoa *pessoaMaisVelha(Pessoa *vetor, int qtd)
{
    Pessoa *maior = NULL;
    Pessoa *p;

    /* percorrendo o vetor com aritmética de ponteiros */
    for (p = vetor; p < vetor + qtd; p++) {
        if (maior == NULL || p->idade > maior->idade) {
            maior = p;
        }
    }
    return maior;
}

Pessoa *pessoaMaisNova(Pessoa *vetor, int qtd)
{
    Pessoa *menor = NULL;
    int i;

    for (i = 0; i < qtd; i++) {
        if (menor == NULL || vetor[i].idade < menor->idade) {
            menor = &vetor[i];
        }
    }
    return menor;
}

float mediaIdades(const Pessoa *vetor, int qtd)
{
    int i, soma = 0;

    if (qtd == 0) {
        return 0;
    }
    for (i = 0; i < qtd; i++) {
        soma += vetor[i].idade;
    }
    return (float)soma / qtd;
}
