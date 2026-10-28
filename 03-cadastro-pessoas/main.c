/*
 * ============================================================
 *  Programação Estruturada em C - UNIP
 *  Tópico 8 - Ponteiros e manipulação de arquivos
 *  Etapa 03: Projeto prático - Cadastro de Pessoas
 * ============================================================
 *
 *  Arquivos do projeto:
 *    main.c     -> menu e fluxo do programa
 *    pessoa.c   -> operações sobre Pessoa (usa ponteiros)
 *    arquivo.c  -> gravação/leitura em arquivos texto e binário
 *    entrada.c  -> leitura segura do teclado
 *
 *  Ao iniciar, o programa carrega "pessoas.dat" (se existir).
 *  Ao sair, grava tudo de volta em "pessoas.dat".
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "pessoa.h"
#include "arquivo.h"
#include "entrada.h"

/* ---------- Opções do menu ---------- */

void opcaoCadastrar(Pessoa *vetor, int *qtd)
{
    Pessoa nova;

    printf("--- CADASTRAR PESSOA ---\n\n");
    if (*qtd >= MAX_PESSOAS) {
        printf("Cadastro cheio (máximo %d pessoas).\n", MAX_PESSOAS);
        return;
    }

    lerPessoa(&nova);                      /* passa o ENDEREÇO de nova */
    adicionarPessoa(vetor, qtd, &nova);    /* qtd já é um ponteiro aqui */
    printf("\n%s cadastrada com sucesso!\n", nova.nome);
}

void opcaoBuscar(const Pessoa *vetor, int qtd)
{
    char trecho[TAM_NOME];
    int pos, achou = 0;

    printf("--- BUSCAR POR NOME ---\n\n");
    lerTexto("Digite o nome (ou parte dele): ", trecho, TAM_NOME);
    printf("\n");

    pos = buscarPorNome(vetor, qtd, trecho, 0);
    while (pos != -1) {
        if (!achou) {
            exibirCabecalho();
            achou = 1;
        }
        exibirPessoa(&vetor[pos], pos + 1);
        pos = buscarPorNome(vetor, qtd, trecho, pos + 1);  /* continua após a última */
    }

    if (!achou) {
        printf("Nenhuma pessoa encontrada com \"%s\".\n", trecho);
    }
}

/* Pede o número de uma pessoa da lista e devolve o índice (ou -1) */
int escolherPessoa(const Pessoa *vetor, int qtd)
{
    int numero;

    listarPessoas(vetor, qtd);
    if (qtd == 0) {
        return -1;
    }
    numero = lerInteiro("\nNúmero da pessoa (0 para cancelar): ");
    if (numero < 1 || numero > qtd) {
        printf("Operação cancelada.\n");
        return -1;
    }
    return numero - 1;
}

void opcaoAlterar(Pessoa *vetor, int qtd)
{
    int indice;
    Pessoa *p;

    printf("--- ALTERAR PESSOA ---\n\n");
    indice = escolherPessoa(vetor, qtd);
    if (indice == -1) {
        return;
    }

    p = &vetor[indice];      /* p aponta direto para a pessoa DENTRO do vetor */
    printf("\nDados atuais: %s, %d anos\n", p->nome, p->idade);
    printf("Digite os novos dados:\n");
    lerPessoa(p);            /* altera o vetor sem precisar copiar nada */
    printf("\nPessoa alterada com sucesso!\n");
}

void opcaoExcluir(Pessoa *vetor, int *qtd)
{
    int indice;
    char resposta[10];

    printf("--- EXCLUIR PESSOA ---\n\n");
    indice = escolherPessoa(vetor, *qtd);
    if (indice == -1) {
        return;
    }

    printf("\nConfirma a exclusão de %s? (S/N): ", vetor[indice].nome);
    lerTexto("", resposta, sizeof(resposta));
    if (resposta[0] == 'S' || resposta[0] == 's') {
        removerPessoa(vetor, qtd, indice);
        printf("Pessoa excluída.\n");
    } else {
        printf("Exclusão cancelada.\n");
    }
}

void opcaoEstatisticas(Pessoa *vetor, int qtd)
{
    Pessoa *velha, *nova;

    printf("--- ESTATÍSTICAS ---\n\n");
    if (qtd == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
        return;
    }

    velha = pessoaMaisVelha(vetor, qtd);   /* recebe um PONTEIRO */
    nova  = pessoaMaisNova(vetor, qtd);

    printf("Total de pessoas : %d\n", qtd);
    printf("Média de idade   : %.1f anos\n", mediaIdades(vetor, qtd));
    printf("Mais velha       : %s (%d anos) - posição %d\n",
           velha->nome, velha->idade, (int)(velha - vetor) + 1);
    printf("Mais nova        : %s (%d anos) - posição %d\n",
           nova->nome, nova->idade, (int)(nova - vetor) + 1);
    printf("\n(posição calculada por subtração de ponteiros: velha - vetor)\n");
}

void opcaoExportar(const Pessoa *vetor, int qtd)
{
    int n;

    printf("--- EXPORTAR PARA TEXTO ---\n\n");
    n = exportarTexto(ARQ_EXPORT, vetor, qtd);
    if (n == -1) {
        printf("Erro ao criar o arquivo %s.\n", ARQ_EXPORT);
    } else {
        printf("%d pessoa(s) exportada(s) para '%s'.\n", n, ARQ_EXPORT);
        printf("Abra o arquivo no Bloco de Notas para conferir.\n");
    }
}

void opcaoImportar(Pessoa *vetor, int *qtd)
{
    char nomeArquivo[150];
    int n;

    printf("--- IMPORTAR DE TEXTO ---\n\n");
    printf("Formato esperado: uma pessoa por linha -> nome;idade\n");
    lerTexto("Nome do arquivo (ENTER = exemplo_pessoas.txt): ", nomeArquivo, sizeof(nomeArquivo));
    if (nomeArquivo[0] == '\0') {
        sprintf(nomeArquivo, "exemplo_pessoas.txt");
    }

    n = importarTexto(nomeArquivo, vetor, qtd);
    if (n == -1) {
        printf("Erro: não foi possível abrir '%s'.\n", nomeArquivo);
    } else {
        printf("%d pessoa(s) importada(s) de '%s'.\n", n, nomeArquivo);
    }
}

int menu(void)
{
    printf("========================================\n");
    printf("         CADASTRO DE PESSOAS\n");
    printf("========================================\n");
    printf(" 1 - Cadastrar pessoa\n");
    printf(" 2 - Listar pessoas\n");
    printf(" 3 - Buscar por nome\n");
    printf(" 4 - Alterar pessoa\n");
    printf(" 5 - Excluir pessoa\n");
    printf(" 6 - Estatísticas\n");
    printf(" 7 - Exportar para texto (%s)\n", ARQ_EXPORT);
    printf(" 8 - Importar de arquivo texto\n");
    printf(" 9 - Salvar agora (%s)\n", ARQ_DADOS);
    printf(" 0 - Sair (salva automaticamente)\n");
    printf("----------------------------------------\n");
    return lerInteiro("Opção: ");
}

int main(void)
{
    Pessoa pessoas[MAX_PESSOAS];
    int qtd = 0;
    int opcao, n;

    system("chcp 1252 > nul");     /* acentos na entrada e saída do console */
    setlocale(LC_ALL, "Portuguese");

    n = carregarBinario(ARQ_DADOS, pessoas, &qtd);   /* &qtd: o main recebe o total */
    if (n == -1) {
        printf("Arquivo %s não encontrado: iniciando cadastro vazio.\n", ARQ_DADOS);
    } else {
        printf("%d pessoa(s) carregada(s) de %s.\n", n, ARQ_DADOS);
    }
    pausar();

    do {
        system("cls");
        opcao = menu();
        system("cls");

        switch (opcao) {
            case 1: opcaoCadastrar(pessoas, &qtd);    break;
            case 2: printf("--- LISTA DE PESSOAS ---\n\n");
                    listarPessoas(pessoas, qtd);      break;
            case 3: opcaoBuscar(pessoas, qtd);        break;
            case 4: opcaoAlterar(pessoas, qtd);       break;
            case 5: opcaoExcluir(pessoas, &qtd);      break;
            case 6: opcaoEstatisticas(pessoas, qtd);  break;
            case 7: opcaoExportar(pessoas, qtd);      break;
            case 8: opcaoImportar(pessoas, &qtd);     break;
            case 9:
            case 0:
                n = salvarBinario(ARQ_DADOS, pessoas, qtd);
                if (n == -1) {
                    printf("Erro ao salvar %s!\n", ARQ_DADOS);
                } else {
                    printf("%d pessoa(s) salva(s) em %s.\n", n, ARQ_DADOS);
                }
                if (opcao == 0) {
                    printf("Até a próxima!\n");
                }
                break;
            default:
                printf("Opção inválida!\n");
        }

        if (opcao != 0) {
            pausar();
        }
    } while (opcao != 0);

    return 0;
}
