/*
 * ============================================================
 *  Programação Estruturada em C - UNIP
 *  Tópico 8.1 - Definição e uso de ponteiros em C
 *  Etapa 01: Ponteiros (passo a passo)
 * ============================================================
 *
 *  Um PONTEIRO é uma variável que guarda um ENDEREÇO de memória
 *  (o lugar onde outra variável está guardada).
 *
 *    int  idade = 20;    // variável comum: guarda um VALOR
 *    int *p     = &idade; // ponteiro: guarda o ENDEREÇO de idade
 *
 *    &idade  -> "endereço de idade"
 *    *p      -> "valor que está no endereço guardado em p"
 *
 *  Execute o programa e acompanhe cada demonstração.
 *  Cada uma está em uma função separada, na ordem da aula.
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

/* Estrutura usada em toda a disciplina */
typedef struct {
    char nome[100];
    int idade;
} Pessoa;

/* ---------- Funções auxiliares ---------- */

void pausar(void)
{
    printf("\nPressione ENTER para continuar...");
    getchar();
    system("cls");
}

/* ---------- Demonstrações ---------- */

/* 1) Toda variável tem um valor E um endereço */
void demo1_enderecos(void)
{
    int idade = 20;
    float altura = 1.75f;

    printf("=== 1) VALOR x ENDEREÇO ===\n\n");
    printf("idade   -> valor: %d \t endereço (&idade): %p\n", idade, (void *)&idade);
    printf("altura  -> valor: %.2f \t endereço (&altura): %p\n", altura, (void *)&altura);
    printf("\nO operador & devolve o ENDEREÇO da variável.\n");
    printf("Você já usou & antes, no scanf(\"%%d\", &idade)!\n");
    printf("O scanf precisa saber ONDE guardar o valor lido.\n");
}

/* 2) Declarando um ponteiro e usando o operador * */
void demo2_declaracao(void)
{
    int idade = 20;
    int *p;          /* p é um "ponteiro para int" */

    p = &idade;      /* p recebe o endereço de idade */

    printf("=== 2) DECLARANDO E USANDO UM PONTEIRO ===\n\n");
    printf("int idade = 20;\nint *p = &idade;\n\n");
    printf("&idade = %p  (endereço de idade)\n", (void *)&idade);
    printf("p      = %p  (p guarda o mesmo endereço)\n", (void *)p);
    printf("*p     = %d          (valor que está naquele endereço)\n", *p);

    *p = 35;         /* altera idade ATRAVÉS do ponteiro */

    printf("\nApós executar  *p = 35;\n");
    printf("idade  = %d   <- mudou sem usarmos o nome 'idade'!\n", idade);
}

/* 3) Passagem por VALOR x por REFERÊNCIA (endereço) */

void aniversarioPorValor(int idade)
{
    idade = idade + 1;   /* altera apenas a CÓPIA local */
}

void aniversarioPorReferencia(int *idade)
{
    *idade = *idade + 1; /* altera a variável ORIGINAL */
}

void trocar(int *a, int *b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
}

void demo3_passagemParametros(void)
{
    int idade = 20;
    int x = 10, y = 99;

    printf("=== 3) PASSAGEM POR VALOR x POR REFERÊNCIA ===\n\n");

    aniversarioPorValor(idade);
    printf("Após aniversarioPorValor(idade):        idade = %d (não mudou)\n", idade);

    aniversarioPorReferencia(&idade);
    printf("Após aniversarioPorReferencia(&idade):  idade = %d (mudou!)\n", idade);

    printf("\nAntes de trocar(&x, &y): x = %d, y = %d\n", x, y);
    trocar(&x, &y);
    printf("Depois de trocar:        x = %d, y = %d\n", x, y);
    printf("\nUma função só consegue alterar variáveis de quem a chamou\n");
    printf("se receber o ENDEREÇO delas (um ponteiro).\n");
}

/* 4) Ponteiros e vetores */
void demo4_vetores(void)
{
    int idades[5] = {18, 22, 35, 41, 60};
    int *p = idades;   /* o nome do vetor já é o endereço do 1º elemento */
    int i;

    printf("=== 4) PONTEIROS E VETORES ===\n\n");
    printf("idades == &idades[0] ? %s\n\n", (idades == &idades[0]) ? "SIM" : "NÃO");

    printf("i | idades[i] | *(p + i) | endereço (p + i)\n");
    printf("--+-----------+----------+-----------------\n");
    for (i = 0; i < 5; i++) {
        printf("%d | %9d | %8d | %p\n", i, idades[i], *(p + i), (void *)(p + i));
    }
    printf("\nRepare: cada endereço avança %d bytes = sizeof(int).\n", (int)sizeof(int));
    printf("p + 1 não soma 1 byte: avança 1 ELEMENTO do vetor.\n");
}

/* 5) Ponteiro para struct: operador -> */
void demo5_ponteiroStruct(void)
{
    Pessoa pessoa = {"Ana Souza", 19};
    Pessoa *p = &pessoa;

    printf("=== 5) PONTEIRO PARA STRUCT (operador ->) ===\n\n");
    printf("Pessoa pessoa = {\"Ana Souza\", 19};\nPessoa *p = &pessoa;\n\n");
    printf("pessoa.nome  = %s\n", pessoa.nome);
    printf("(*p).nome    = %s\n", (*p).nome);
    printf("p->nome      = %s   <- forma mais usada\n", p->nome);

    p->idade = 20;                 /* mesmo que (*p).idade = 20 */
    strcpy(p->nome, "Ana Souza Lima");

    printf("\nApós p->idade = 20 e strcpy(p->nome, \"Ana Souza Lima\"):\n");
    printf("pessoa.nome = %s, pessoa.idade = %d\n", pessoa.nome, pessoa.idade);
    printf("\nsizeof(Pessoa) = %d bytes | sizeof(Pessoa *) = %d bytes\n",
           (int)sizeof(Pessoa), (int)sizeof(Pessoa *));
    printf("Passar um ponteiro é mais leve do que copiar a struct inteira.\n");
}

/* 6) Lendo uma string (nome) com fgets e preenchendo a struct via ponteiro */

/* Lê uma linha do teclado e remove o '\n' que o fgets guarda no final */
void lerTexto(char *destino, int tamanho)
{
    if (fgets(destino, tamanho, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    }
}

void lerPessoa(Pessoa *p)
{
    printf("Nome: ");
    lerTexto(p->nome, sizeof(p->nome));   /* p->nome já é um endereço (vetor) */

    printf("Idade: ");
    scanf("%d", &p->idade);               /* &p->idade = endereço do campo */
    while (getchar() != '\n');            /* limpa o ENTER que sobrou no teclado */
}

void demo6_lerPessoa(void)
{
    Pessoa pessoa;

    printf("=== 6) PREENCHENDO UMA STRUCT ATRAVÉS DE PONTEIRO ===\n\n");
    printf("A função lerPessoa(Pessoa *p) recebe o ENDEREÇO da struct.\n\n");

    lerPessoa(&pessoa);

    printf("\nDentro do main: %s tem %d anos.\n", pessoa.nome, pessoa.idade);
    printf("\nObs.: para ler o nome usamos fgets (lê a linha toda, com espaços).\n");
    printf("scanf(\"%%s\") pararia no primeiro espaço: \"Ana Souza\" viraria \"Ana\".\n");
}

/* 7) Alocação dinâmica: malloc devolve um ponteiro */
void demo7_alocacaoDinamica(void)
{
    int qtd, i;
    Pessoa *turma;   /* vai apontar para um vetor criado em tempo de execução */

    printf("=== 7) ALOCAÇÃO DINÂMICA (malloc / free) ===\n\n");
    printf("Quantas pessoas deseja cadastrar? ");
    scanf("%d", &qtd);
    while (getchar() != '\n');

    if (qtd <= 0) {
        printf("Quantidade inválida.\n");
        return;
    }

    turma = (Pessoa *) malloc(qtd * sizeof(Pessoa));
    if (turma == NULL) {           /* SEMPRE testar: pode faltar memória */
        printf("Erro: memória insuficiente!\n");
        return;
    }

    for (i = 0; i < qtd; i++) {
        printf("\n-- Pessoa %d --\n", i + 1);
        lerPessoa(&turma[i]);      /* &turma[i] é o mesmo que (turma + i) */
    }

    printf("\n%-30s | %s\n", "NOME", "IDADE");
    for (i = 0; i < qtd; i++) {
        printf("%-30s | %d\n", turma[i].nome, turma[i].idade);
    }

    free(turma);                   /* devolve a memória ao sistema */
    turma = NULL;                  /* boa prática: evita "ponteiro solto" */
    printf("\nMemória liberada com free().\n");
}

int main(void)
{
    system("chcp 1252 > nul");     /* acentos na entrada e saída do console */
    setlocale(LC_ALL, "Portuguese");

    demo1_enderecos();           pausar();
    demo2_declaracao();          pausar();
    demo3_passagemParametros();  pausar();
    demo4_vetores();             pausar();
    demo5_ponteiroStruct();      pausar();
    demo6_lerPessoa();           pausar();
    demo7_alocacaoDinamica();

    printf("\nFim da etapa 01. Próxima: 02-arquivos.\n");
    return 0;
}
