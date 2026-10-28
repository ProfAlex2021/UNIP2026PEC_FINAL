# Ponteiros e Manipulação de Arquivos em C

**Programação Estruturada em C** · Análise e Desenvolvimento de Sistemas · UNIP (presencial)

Projeto prático dos tópicos da ementa:

- **8.1** Definição e uso de ponteiros em C
- **8.2** Manipulação de arquivos

Todos os programas rodam no terminal (CLI) e foram feitos para o **Code::Blocks** com o compilador **GNU GCC**.

---

## Como abrir no Code::Blocks

1. Clone ou baixe este repositório.
2. No Code::Blocks: **File → Open...** e escolha **`PEC-Ponteiros-Arquivos.workspace`**.
   Os três projetos aparecem no painel *Projects*.
3. Para trocar de projeto, clique com o botão direito sobre ele → **Activate project**.
4. Compile e execute com **F9** (*Build and run*).

> Também dá para abrir cada projeto sozinho pelo arquivo `.cbp` da pasta correspondente.

---

## Estrutura do repositório

```
PEC/
├── PEC-Ponteiros-Arquivos.workspace   ← abra este arquivo no Code::Blocks
├── 01-ponteiros/                      ← tópico 8.1
│   ├── 01-ponteiros.cbp
│   └── main.c
├── 02-arquivos/                       ← tópico 8.2
│   ├── 02-arquivos.cbp
│   └── main.c
└── 03-cadastro-pessoas/               ← projeto prático (8.1 + 8.2)
    ├── 03-cadastro-pessoas.cbp
    ├── main.c               menu e fluxo do programa
    ├── pessoa.h / pessoa.c  struct Pessoa e operações com ponteiros
    ├── arquivo.h / arquivo.c gravação e leitura (texto e binário)
    ├── entrada.h / entrada.c leitura segura do teclado
    └── exemplo_pessoas.txt  dados de exemplo para importar
```

---

## A estrutura de dados: `Pessoa`

Todo o projeto gira em torno desta estrutura:

```c
typedef struct {
    char nome[100];
    int idade;
} Pessoa;
```

> **Atenção à sintaxe.** Duas formas comuns de erro ao escrever essa declaração:
>
> | Errado | Certo | Por quê |
> |---|---|---|
> | `struct typedef Pessoa { ... };` | `typedef struct { ... } Pessoa;` | `typedef` vem antes de `struct`, e o novo nome do tipo vem **depois** das chaves |
> | `char[100] nome;` | `char nome[100];` | em C o tamanho do vetor fica depois do nome da variável |

---

## Roteiro das aulas

### Etapa 01 – Ponteiros (`01-ponteiros`)

Programa demonstrativo em 7 passos (o aluno aperta ENTER entre eles):

| Passo | Conceito | O que observar |
|---|---|---|
| 1 | Valor × endereço, operador `&` | O `&` do `scanf` finalmente faz sentido |
| 2 | Declaração `int *p`, operador `*` | Alterar `*p` altera a variável original |
| 3 | Passagem por valor × por referência | `aniversarioPorValor` não muda nada; `trocar(&x, &y)` funciona |
| 4 | Ponteiros e vetores | `vetor[i]` é o mesmo que `*(p + i)`; endereços avançam `sizeof(int)` |
| 5 | Ponteiro para struct, operador `->` | `p->nome` é o mesmo que `(*p).nome` |
| 6 | Função que preenche uma `Pessoa` via ponteiro | Leitura de *string* com `fgets` |
| 7 | Alocação dinâmica `malloc` / `free` | Vetor de `Pessoa` com tamanho escolhido pelo usuário |

### Etapa 02 – Arquivos (`02-arquivos`)

| Passo | Conceito | Funções |
|---|---|---|
| 1 | Criar arquivo texto | `fopen("w")`, `fprintf`, `fclose` |
| 2 | Acrescentar no final | `fopen("a")` |
| 3 | Ler linha a linha | `fopen("r")`, `fgets` |
| 4 | Separar campos de uma linha | `sscanf(linha, "%99[^;];%d", ...)` |
| 5 | Gravar arquivo binário | `fopen("wb")`, `fwrite` |
| 6 | Ler arquivo binário | `fopen("rb")`, `fread` |
| 7 | Acesso direto a um registro | `fseek`, `ftell` |

Depois de executar, abra `pessoas.txt` e `pessoas.dat` no Bloco de Notas e compare:
o primeiro é legível, o segundo não.

### Etapa 03 – Projeto prático: Cadastro de Pessoas (`03-cadastro-pessoas`)

Sistema com menu que junta os dois tópicos:

```
 1 - Cadastrar pessoa
 2 - Listar pessoas
 3 - Buscar por nome
 4 - Alterar pessoa
 5 - Excluir pessoa
 6 - Estatísticas
 7 - Exportar para texto (pessoas.txt)
 8 - Importar de arquivo texto
 9 - Salvar agora (pessoas.dat)
 0 - Sair (salva automaticamente)
```

- Ao iniciar, carrega `pessoas.dat` (binário). Ao sair, grava de volta.
- Use a opção **8** e aperte ENTER para importar `exemplo_pessoas.txt` e ter dados para testar.

**Onde os ponteiros aparecem no projeto:**

| Situação | Exemplo no código |
|---|---|
| Função altera variável de quem chamou | `adicionarPessoa(vetor, &qtd, &nova)` altera `qtd` do `main` |
| Função só lê (sem copiar a struct) | `exibirPessoa(const Pessoa *p, int numero)` |
| Alterar um elemento direto no vetor | `Pessoa *p = &vetor[indice]; lerPessoa(p);` |
| Função devolve um ponteiro | `Pessoa *pessoaMaisVelha(...)` devolve `NULL` se o vetor estiver vazio |
| Aritmética de ponteiros | `for (p = vetor; p < vetor + qtd; p++)` e `velha - vetor` para achar a posição |
| Ponteiro devolvido por função da biblioteca | `strchr`, `strstr` e `fopen` (`FILE *`) |

---

## Bibliotecas usadas

Além das que a turma já conhece (`stdio.h`, `stdlib.h`, `locale.h`), estes programas usam
**`string.h`**, para trabalhar com o campo `nome`:

| Função | Uso |
|---|---|
| `strcpy(destino, origem)` | copia uma string |
| `strlen(s)` | tamanho da string |
| `strcspn(s, "\n")` | posição do primeiro `\n` (para removê-lo após o `fgets`) |
| `strchr(s, c)` | ponteiro para o primeiro caractere `c`, ou `NULL` |
| `strstr(s, trecho)` | ponteiro para onde `trecho` aparece em `s`, ou `NULL` |

### Lendo strings do teclado

Como a turma ainda não leu *strings* pelo teclado, este projeto apresenta o `fgets`:

```c
fgets(p->nome, sizeof(p->nome), stdin);   // lê a linha inteira, com espaços
p->nome[strcspn(p->nome, "\n")] = '\0';    // remove o ENTER do final
```

- `scanf("%s", nome)` para no primeiro espaço: "Ana Souza" vira "Ana".
- Depois de um `scanf("%d", ...)`, o ENTER fica no teclado e o próximo `fgets` lê uma linha vazia.
  Por isso, nas etapas 01 e 02 usamos `while (getchar() != '\n');` logo após o `scanf`,
  e na etapa 03 todas as leituras passam pelo `fgets` (veja `entrada.c`).

---

## Acentuação no terminal

Os códigos-fonte estão em **UTF-8** (bom para o GitHub). Para os acentos saírem certos no
terminal do Windows, cada projeto já vem configurado com:

- Opções do compilador (em *Project → Build options → Compiler settings → Other compiler options*):
  `-finput-charset=UTF-8` e `-fexec-charset=CP1252`
- No início do `main`:
  ```c
  system("chcp 1252 > nul");        // terminal em Windows-1252 (entrada e saída)
  setlocale(LC_ALL, "Portuguese");
  ```

Os arquivos `pessoas.txt` gerados pelos programas ficam em Windows-1252 (ANSI), que o Bloco de Notas abre normalmente.

---

## Exercícios propostos

**Ponteiros**

1. Escreva `void dobrar(int *n)` que dobra o valor da variável recebida.
2. Escreva `void minMax(int *vetor, int tam, int *menor, int *maior)` que devolve, pelos ponteiros, o menor e o maior valor do vetor.
3. Escreva `void maiusculas(char *texto)` que converte o texto para maiúsculas percorrendo-o com um ponteiro (`while (*texto != '\0')`).

**Arquivos**

4. Na etapa 02, conte quantas pessoas do `pessoas.txt` são maiores de idade.
5. Na etapa 02, use `fseek` + `fwrite` em modo `"r+b"` para alterar a idade de um registro do `pessoas.dat` sem regravar o arquivo inteiro.

**Projeto (etapa 03)**

6. Acrescente a opção **"Listar maiores de idade"**.
7. Acrescente a opção **"Ordenar por nome"** (dica: `strcmp` e a função `trocar` com `Pessoa *`).
8. Faça a busca por nome ignorar maiúsculas e minúsculas.
9. Acrescente o campo `char email[60];` na struct. O que acontece ao carregar um `pessoas.dat` antigo? Por quê?
   *(Discussão: o tamanho de cada registro do arquivo binário depende de `sizeof(Pessoa)`.)*
10. **Desafio:** troque o vetor fixo `Pessoa pessoas[MAX_PESSOAS]` por um vetor alocado com `malloc` que cresce com `realloc` quando fica cheio.

---

## Arquivos ignorados pelo Git

O `.gitignore` deixa de fora as pastas `bin/` e `obj/`, os arquivos pessoais do Code::Blocks
(`*.layout`, `*.depend`) e os arquivos criados ao executar os programas (`pessoas.txt`, `pessoas.dat`).
