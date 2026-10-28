/*
 * arquivo.h - Persistência (gravação e leitura) do cadastro.
 *
 *  Binário (.dat): usado para salvar/carregar o cadastro automaticamente.
 *  Texto   (.txt): usado para exportar/importar (legível no Bloco de Notas).
 *
 * As funções de leitura recebem 'int *qtd' para devolver
 * ao main quantas pessoas foram carregadas.
 */
#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "pessoa.h"

#define ARQ_DADOS   "pessoas.dat"
#define ARQ_EXPORT  "pessoas.txt"

int salvarBinario(const char *nomeArquivo, const Pessoa *vetor, int qtd);
int carregarBinario(const char *nomeArquivo, Pessoa *vetor, int *qtd);

int exportarTexto(const char *nomeArquivo, const Pessoa *vetor, int qtd);
int importarTexto(const char *nomeArquivo, Pessoa *vetor, int *qtd);

#endif
