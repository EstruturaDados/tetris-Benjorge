#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5  // Capacidade máxima da fila

// Estrutura da peça
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Insere uma peça no final da fila
void inserir(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia. Não é possível inserir nova peça.\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

// Remove a peça da frente da fila
Peca remover(Fila *f) {
    Peca vazia = {'-', -1};
    if (filaVazia(f)) {
        printf("Fila vazia. Não há peça para jogar.\n");
        return vazia;
    }
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
    return p;
}

// Exibe o estado atual da fila
void mostrarFila(Fila *f) {
    printf("Fila de peças: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

// Menu principal
int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL));  // Inicializa gerador de números aleatórios

    int opcao, contadorID = 0;

    // Preenche a fila inicialmente
    for (int i = 0; i < MAX; i++) {
        inserir(&fila, gerarPeca(contadorID++));
    }

    do {
        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = remover(&fila);
                if (jogada.id != -1) {
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                }
                break;
            }
            case 2: {
                inserir(&fila, gerarPeca(contadorID++));
                break;
            }
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        mostrarFila(&fila);

    } while (opcao != 0);

    return 0;
}