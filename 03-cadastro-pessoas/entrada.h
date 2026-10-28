/*
 * entrada.h - Funções auxiliares para leitura do teclado.
 */
#ifndef ENTRADA_H
#define ENTRADA_H

void limparBuffer(void);
void lerTexto(const char *mensagem, char *destino, int tamanho);
int  lerInteiro(const char *mensagem);
void pausar(void);

#endif
