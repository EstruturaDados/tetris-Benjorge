#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura da peça
typedef struct {
    char nome;
    int id;
} Peca;

// Fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio, fim, total;
} Fila;

// Pilha linear
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// Inicialização
void inicializarFila(Fila *f) {
    f->inicio = f->fim = f->total = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verificações
int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Geração de peça
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova = {tipos[rand() % 4], id};
    return nova;
}

// Operações da fila
void inserirFila(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % MAX_FILA;
        f->total++;
    }
}

Peca removerFila(Fila *f) {
    Peca vazia = {'-', -1};
    if (filaVazia(f)) return vazia;
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return p;
}

// Operações da pilha
void push(Pilha *p, Peca nova) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->itens[p->topo] = nova;
    } else {
        printf("Pilha cheia. Não é possível reservar nova peça.\n");
    }
}

Peca pop(Pilha *p) {
    Peca vazia = {'-', -1};
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Não há peça reservada para usar.\n");
        return vazia;
    }
    return p->itens[p->topo--];
}

// Exibição
void mostrarFila(Fila *f) {
    printf("Fila de peças\t");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\n");
}

void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva\t(Topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// Troca direta entre frente da fila e topo da pilha
void trocarTopoComFrente(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Não é possível realizar a troca. Fila ou pilha vazia.\n");
        return;
    }
    int idx = f->inicio;
    Peca temp = f->itens[idx];
    f->itens[idx] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("Troca realizada entre frente da fila e topo da pilha.\n");
}

// Troca múltipla entre os 3 primeiros da fila e os 3 da pilha
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->total < 3 || p->topo < 2) {
        printf("Não é possível realizar troca múltipla. Faltam peças.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idx = (f->inicio + i) % MAX_FILA;
        Peca temp = f->itens[idx];
        f->itens[idx] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }
    printf("Troca múltipla realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// Programa principal
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
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
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
            case 4:
                trocarTopoComFrente(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        // Exibe estado atual
        printf("\nEstado atual:\n");
        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (opcao != 0);

    return 0;
}