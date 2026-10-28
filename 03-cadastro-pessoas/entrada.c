/*
 * entrada.c - Leitura segura do teclado.
 *
 * Por que não usar só scanf?
 *   - scanf("%s") para no primeiro espaço ("Ana Souza" -> "Ana");
 *   - scanf("%d") deixa o ENTER no teclado e atrapalha a próxima leitura.
 * Por isso lemos SEMPRE a linha inteira com fgets.
 */
#include <stdio.h>
#include <string.h>
#include "entrada.h"

/* Descarta tudo o que sobrou no teclado até o ENTER */
void limparBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Mostra a mensagem e lê uma linha para 'destino'.
 * 'destino' é um ponteiro: a função escreve direto na variável de quem chamou.
 */
void lerTexto(const char *mensagem, char *destino, int tamanho)
{
    char *fimDaLinha;

    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }

    fimDaLinha = strchr(destino, '\n');   /* ponteiro para o '\n' ou NULL */
    if (fimDaLinha != NULL) {
        *fimDaLinha = '\0';               /* troca o '\n' pelo fim da string */
    } else {
        limparBuffer();                   /* digitou mais do que cabia */
    }

    /* não aceita ';' no texto, pois ele separa os campos no arquivo */
    while ((fimDaLinha = strchr(destino, ';')) != NULL) {
        *fimDaLinha = ',';
    }
}

/* Repete a pergunta até o usuário digitar um número inteiro válido */
int lerInteiro(const char *mensagem)
{
    char linha[50];
    int valor;

    while (1) {
        lerTexto(mensagem, linha, sizeof(linha));
        if (sscanf(linha, "%d", &valor) == 1) {
            return valor;
        }
        printf("Digite um número inteiro.\n");
    }
}

void pausar(void)
{
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
}
