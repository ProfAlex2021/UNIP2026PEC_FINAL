/*
 * ============================================================
 *  Programação Estruturada em C - UNIP
 *  Tópico 8.2 - Manipulação de arquivos
 *  Etapa 02: Arquivos texto e binário (passo a passo)
 * ============================================================
 *
 *  Para usar um arquivo em C usamos um PONTEIRO do tipo FILE:
 *
 *      FILE *arq = fopen("nome.txt", "modo");
 *
 *  Modos principais de abertura:
 *      "w"  escrita  (cria o arquivo; se já existir, APAGA o conteúdo)
 *      "a"  acréscimo (append: escreve no FINAL do arquivo)
 *      "r"  leitura  (o arquivo precisa existir)
 *      "wb" / "ab" / "rb"  os mesmos modos, em formato BINÁRIO
 *
 *  Regras de ouro:
 *      1) SEMPRE testar se fopen devolveu NULL (falhou).
 *      2) SEMPRE fechar com fclose(arq) ao terminar.
 *
 *  Os arquivos são criados na pasta do projeto (02-arquivos).
 *  Depois de executar, abra pessoas.txt no Bloco de Notas!
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define ARQ_TEXTO   "pessoas.txt"
#define ARQ_BINARIO "pessoas.dat"

typedef struct {
    char nome[100];
    int idade;
} Pessoa;

void pausar(void)
{
    printf("\nPressione ENTER para continuar...");
    getchar();
    system("cls");
}

void lerTexto(char *destino, int tamanho)
{
    if (fgets(destino, tamanho, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    }
}

/* ------------------------------------------------------------
 * 1) Gravar em arquivo TEXTO com fprintf (modo "w")
 *    fprintf funciona igual ao printf, mas escreve no arquivo.
 *    Formato de cada linha:  nome;idade
 * ------------------------------------------------------------ */
void demo1_gravarTexto(void)
{
    Pessoa lista[3] = {
        {"Ana Souza", 19},
        {"Bruno Lima", 23},
        {"Carla Mendes", 31}
    };
    FILE *arq;
    int i;

    printf("=== 1) GRAVANDO EM ARQUIVO TEXTO (fopen \"w\" + fprintf) ===\n\n");

    arq = fopen(ARQ_TEXTO, "w");
    if (arq == NULL) {
        printf("Erro ao criar o arquivo %s\n", ARQ_TEXTO);
        return;
    }

    for (i = 0; i < 3; i++) {
        fprintf(arq, "%s;%d\n", lista[i].nome, lista[i].idade);
        printf("Gravado: %s;%d\n", lista[i].nome, lista[i].idade);
    }

    fclose(arq);
    printf("\nArquivo '%s' criado. Abra-o no Bloco de Notas para conferir.\n", ARQ_TEXTO);
}

/* ------------------------------------------------------------
 * 2) Acrescentar no final com o modo "a" (append)
 * ------------------------------------------------------------ */
void demo2_acrescentarTexto(void)
{
    Pessoa nova;
    FILE *arq;

    printf("=== 2) ACRESCENTANDO NO FINAL (fopen \"a\") ===\n\n");
    printf("Digite os dados de uma nova pessoa:\n");
    printf("Nome: ");
    lerTexto(nova.nome, sizeof(nova.nome));
    printf("Idade: ");
    scanf("%d", &nova.idade);
    while (getchar() != '\n');

    arq = fopen(ARQ_TEXTO, "a");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s\n", ARQ_TEXTO);
        return;
    }
    fprintf(arq, "%s;%d\n", nova.nome, nova.idade);
    fclose(arq);

    printf("\n'%s' acrescentada ao final de %s.\n", nova.nome, ARQ_TEXTO);
    printf("Se usássemos \"w\", as pessoas anteriores seriam APAGADAS!\n");
}

/* ------------------------------------------------------------
 * 3) Ler o arquivo TEXTO linha por linha com fgets
 *    fgets devolve NULL quando chega ao fim do arquivo.
 * ------------------------------------------------------------ */
void demo3_lerTextoLinhas(void)
{
    char linha[150];
    int numero = 0;
    FILE *arq;

    printf("=== 3) LENDO O ARQUIVO TEXTO LINHA A LINHA (fgets) ===\n\n");

    arq = fopen(ARQ_TEXTO, "r");
    if (arq == NULL) {
        printf("Erro: o arquivo %s não existe.\n", ARQ_TEXTO);
        return;
    }

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        numero++;
        printf("Linha %d: %s", numero, linha);   /* a linha já contém '\n' */
    }

    fclose(arq);
    printf("\nTotal de linhas lidas: %d\n", numero);
}

/* ------------------------------------------------------------
 * 4) Ler o arquivo TEXTO e separar os campos para uma struct
 *    sscanf funciona como scanf, mas lê de uma string.
 *    "%99[^;]" = leia até 99 caracteres que NÃO sejam ';'
 * ------------------------------------------------------------ */
void demo4_lerTextoParaStruct(void)
{
    char linha[150];
    Pessoa p;
    int soma = 0, qtd = 0;
    FILE *arq;

    printf("=== 4) LENDO O ARQUIVO TEXTO PARA A STRUCT (fgets + sscanf) ===\n\n");

    arq = fopen(ARQ_TEXTO, "r");
    if (arq == NULL) {
        printf("Erro: o arquivo %s não existe.\n", ARQ_TEXTO);
        return;
    }

    printf("%-30s | %s\n", "NOME", "IDADE");
    printf("-------------------------------+------\n");
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        if (sscanf(linha, "%99[^;];%d", p.nome, &p.idade) == 2) {
            printf("%-30s | %d\n", p.nome, p.idade);
            soma += p.idade;
            qtd++;
        }
    }
    fclose(arq);

    if (qtd > 0) {
        printf("\nMédia de idade: %.1f anos\n", (float)soma / qtd);
    }
}

/* ------------------------------------------------------------
 * 5) Gravar em arquivo BINÁRIO com fwrite
 *    fwrite(endereço_dos_dados, tamanho_de_cada_item, quantidade, arquivo)
 *    Grava os bytes da struct exatamente como estão na memória.
 * ------------------------------------------------------------ */
void demo5_gravarBinario(void)
{
    Pessoa lista[3] = {
        {"Diego Rocha", 45},
        {"Elisa Prado", 28},
        {"Fabio Nunes", 52}
    };
    FILE *arq;
    int gravados;

    printf("=== 5) GRAVANDO EM ARQUIVO BINÁRIO (fopen \"wb\" + fwrite) ===\n\n");

    arq = fopen(ARQ_BINARIO, "wb");
    if (arq == NULL) {
        printf("Erro ao criar o arquivo %s\n", ARQ_BINARIO);
        return;
    }

    /* lista é um vetor -> o nome já é um ponteiro para o 1º elemento */
    gravados = fwrite(lista, sizeof(Pessoa), 3, arq);
    fclose(arq);

    printf("%d registros gravados em '%s'.\n", gravados, ARQ_BINARIO);
    printf("Cada registro ocupa sizeof(Pessoa) = %d bytes.\n", (int)sizeof(Pessoa));
    printf("Tamanho esperado do arquivo: %d bytes.\n", gravados * (int)sizeof(Pessoa));
    printf("\nTente abrir pessoas.dat no Bloco de Notas: não é legível!\n");
}

/* ------------------------------------------------------------
 * 6) Ler o arquivo BINÁRIO com fread
 *    fread devolve quantos itens conseguiu ler (0 = fim do arquivo).
 * ------------------------------------------------------------ */
void demo6_lerBinario(void)
{
    Pessoa p;
    FILE *arq;

    printf("=== 6) LENDO O ARQUIVO BINÁRIO (fopen \"rb\" + fread) ===\n\n");

    arq = fopen(ARQ_BINARIO, "rb");
    if (arq == NULL) {
        printf("Erro: o arquivo %s não existe.\n", ARQ_BINARIO);
        return;
    }

    /* &p = endereço onde o fread vai colocar os bytes lidos */
    while (fread(&p, sizeof(Pessoa), 1, arq) == 1) {
        printf("%-30s | %d\n", p.nome, p.idade);
    }

    fclose(arq);
}

/* ------------------------------------------------------------
 * 7) Acesso direto no binário com fseek / ftell
 *    Como todo registro tem o mesmo tamanho, dá para "pular"
 *    direto para o registro desejado.
 * ------------------------------------------------------------ */
void demo7_acessoDireto(void)
{
    Pessoa p;
    FILE *arq;
    long tamanho;
    int total, posicao;

    printf("=== 7) ACESSO DIRETO NO BINÁRIO (fseek / ftell) ===\n\n");

    arq = fopen(ARQ_BINARIO, "rb");
    if (arq == NULL) {
        printf("Erro: o arquivo %s não existe.\n", ARQ_BINARIO);
        return;
    }

    fseek(arq, 0, SEEK_END);           /* vai para o final do arquivo */
    tamanho = ftell(arq);              /* posição atual = tamanho em bytes */
    total = (int)(tamanho / sizeof(Pessoa));
    printf("O arquivo tem %ld bytes = %d registros.\n", tamanho, total);

    printf("Qual registro deseja ler (1 a %d)? ", total);
    scanf("%d", &posicao);
    while (getchar() != '\n');

    if (posicao < 1 || posicao > total) {
        printf("Posição inválida.\n");
    } else {
        /* pula (posicao - 1) registros a partir do início */
        fseek(arq, (posicao - 1) * sizeof(Pessoa), SEEK_SET);
        fread(&p, sizeof(Pessoa), 1, arq);
        printf("\nRegistro %d: %s, %d anos\n", posicao, p.nome, p.idade);
    }

    fclose(arq);
}

int main(void)
{
    system("chcp 1252 > nul");
    setlocale(LC_ALL, "Portuguese");

    demo1_gravarTexto();         pausar();
    demo2_acrescentarTexto();    pausar();
    demo3_lerTextoLinhas();      pausar();
    demo4_lerTextoParaStruct();  pausar();
    demo5_gravarBinario();       pausar();
    demo6_lerBinario();          pausar();
    demo7_acessoDireto();

    printf("\nFim da etapa 02. Próxima: 03-cadastro-pessoas.\n");
    return 0;
}
