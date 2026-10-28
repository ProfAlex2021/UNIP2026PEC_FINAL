/*
 * pessoa.h - Definição da struct Pessoa e das operações sobre ela.
 *
 * Repare que quase todas as funções recebem PONTEIROS:
 *   - Pessoa *p        -> a função pode ALTERAR a pessoa
 *   - const Pessoa *p  -> a função só LÊ (const impede alterações)
 *   - int *qtd         -> a função pode alterar a quantidade do main
 */
#ifndef PESSOA_H
#define PESSOA_H

#define TAM_NOME      100
#define MAX_PESSOAS   100

typedef struct {
    char nome[TAM_NOME];
    int idade;
} Pessoa;

void   lerPessoa(Pessoa *p);
void   exibirCabecalho(void);
void   exibirPessoa(const Pessoa *p, int numero);
void   listarPessoas(const Pessoa *vetor, int qtd);

int    adicionarPessoa(Pessoa *vetor, int *qtd, const Pessoa *nova);
int    buscarPorNome(const Pessoa *vetor, int qtd, const char *trecho, int inicio);
int    removerPessoa(Pessoa *vetor, int *qtd, int indice);

Pessoa *pessoaMaisVelha(Pessoa *vetor, int qtd);
Pessoa *pessoaMaisNova(Pessoa *vetor, int qtd);
float   mediaIdades(const Pessoa *vetor, int qtd);

#endif
