#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5     // Capacidade da fila
#define MAX_PILHA 3    // Capacidade da pilha

// Estrutura da peça
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

// Estrutura da pilha linear
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
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
void inserirFila(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

// Remove a peça da frente da fila
Peca removerFila(Fila *f) {
    Peca vazia = {'-', -1};
    if (filaVazia(f)) return vazia;
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return p;
}

// Insere uma peça no topo da pilha
void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia. Não é possível reservar nova peça.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = nova;
}

// Remove a peça do topo da pilha
Peca pop(Pilha *p) {
    Peca vazia = {'-', -1};
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Não há peça reservada para usar.\n");
        return vazia;
    }
    return p->itens[p->topo--];
}

// Exibe o estado atual da fila
void mostrarFila(Fila *f) {
    printf("Fila de peças: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

// Exibe o estado atual da pilha
void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// Menu principal
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, contadorID = 0;

    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    // Preenche a fila inicialmente
    for (int i = 0; i < MAX_FILA; i++) {
        inserirFila(&fila, gerarPeca(contadorID++));
    }

    do {
        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = removerFila(&fila);
                if (jogada.id != -1) {
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    inserirFila(&fila, gerarPeca(contadorID++));
                }
                break;
            }
            case 2: {
                if (!filaVazia(&fila)) {
                    Peca reservada = removerFila(&fila);
                    push(&pilha, reservada);
                    inserirFila(&fila, gerarPeca(contadorID++));
                }
                break;
            }
            case 3: {
                Peca usada = pop(&pilha);
                if (usada.id != -1) {
                    printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        // Exibe estado atual
        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (opcao != 0);

    return 0;
}