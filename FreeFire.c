#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Para usar o tipo 'bool'

// --- Constantes e Estruturas Globais ---
#define MAX_ITENS 10

// Struct para os dados do item (usada por ambas as estruturas)
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
};

// Struct para o nó da Lista Encadeada
typedef struct No {
    struct Item dados;
    struct No* proximo;
} No;

// --- Protótipos das Funções ---

// Funções do Menu
void menuPrincipal();
void menuVetor(struct Item mochilaVetor[], int* numItens);
void menuLista(No** cabecaLista); // Passa o ponteiro para o ponteiro da cabeça da lista

// Funções para a Mochila com VETOR
void inserirItemVetor(struct Item mochila[], int* numItens);
void removerItemVetor(struct Item mochila[], int* numItens);
void listarItensVetor(struct Item mochila[], int numItens);
void buscarSequencialVetor(struct Item mochila[], int numItens);
void ordenarVetor(struct Item mochila[], int numItens);
void buscarBinariaVetor(struct Item mochila[], int numItens);

// Funções para a Mochila com LISTA ENCADEADA
void inserirItemLista(No** cabeca);
void removerItemLista(No** cabeca);
void listarItensLista(No* cabeca);
void buscarSequencialLista(No* cabeca);
void liberarLista(No** cabeca); // Função para limpar a memória usada pela lista

// Funções Auxiliares
void limparBuffer();

// --- Função Principal ---
int main() {
    menuPrincipal();
    return 0;
}

// --- Implementação dos Menus ---
void menuPrincipal() {
    // Declaração das estruturas de dados
    struct Item mochilaVetor[MAX_ITENS];
    int numItensVetor = 0;
    No* mochilaLista = NULL; // A lista começa vazia
    int opcao;

    do {
        printf("\n--- ESCOLHA O TIPO DE MOCHILA ---\n");
        printf("1. Mochila com Vetor (Rapida para acesso, tamanho fixo)\n");
        printf("2. Mochila com Lista Encadeada (Flexivel, tamanho dinamico)\n");
        printf("0. Sair do Jogo\n");
        printf("-----------------------------------\n");
        printf("Escolha uma opcao: ");

        char input[10];
        fgets(input, 10, stdin);
        if (sscanf(input, "%d", &opcao) != 1) {
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                menuVetor(mochilaVetor, &numItensVetor);
                break;
            case 2:
                menuLista(&mochilaLista);
                break;
            case 0:
                printf("\nObrigado por jogar! Saindo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    // Libera a memória alocada para a lista encadeada antes de sair
    liberarLista(&mochilaLista);
}

void menuVetor(struct Item mochilaVetor[], int* numItens) {
    int opcao;
    bool ordenado = false; // Flag para controlar se o vetor está ordenado

    do {
        printf("\n--- MOCHILA COM VETOR ---\n");
        printf("Status: %d/%d itens. Vetor %s ordenado.\n", *numItens, MAX_ITENS, (ordenado ? "ESTA" : "NAO ESTA"));
        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Ordenar itens por nome (Bubble Sort)\n");
        printf("5. Buscar item (Sequencial)\n");
        printf("6. Buscar item (Binaria) - Requer ordenacao\n");
        printf("0. Voltar ao menu principal\n");
        printf("---------------------------\n");
        printf("Escolha uma opcao: ");

        char input[10];
        fgets(input, 10, stdin);
        if (sscanf(input, "%d", &opcao) != 1) {
            opcao = -1;
        }

        // Qualquer operação que adiciona ou remove, desordena o vetor
        if (opcao == 1 || opcao == 2) {
            ordenado = false;
        }

        switch (opcao) {
            case 1: inserirItemVetor(mochilaVetor, numItens); break;
            case 2: removerItemVetor(mochilaVetor, numItens); break;
            case 3: listarItensVetor(mochilaVetor, *numItens); break;
            case 4:
                ordenarVetor(mochilaVetor, *numItens);
                ordenado = true; // Marca como ordenado
                break;
            case 5: buscarSequencialVetor(mochilaVetor, *numItens); break;
            case 6:
                if (ordenado) {
                    buscarBinariaVetor(mochilaVetor, *numItens);
                } else {
                    printf("\nERRO: A busca binaria requer que o vetor esteja ordenado! Ordene primeiro (opcao 4).\n");
                }
                break;
            case 0: printf("\nVoltando ao menu principal...\n"); break;
            default: printf("\nOpcao invalida!\n"); break;
        }

    } while (opcao != 0);
}

void menuLista(No** cabecaLista) {
    int opcao;
    do {
        printf("\n--- MOCHILA COM LISTA ENCADEADA ---\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item (Sequencial)\n");
        printf("0. Voltar ao menu principal\n");
        printf("-----------------------------------\n");
        printf("Escolha uma opcao: ");

        char input[10];
        fgets(input, 10, stdin);
        if (sscanf(input, "%d", &opcao) != 1) {
            opcao = -1;
        }

        switch (opcao) {
            case 1: inserirItemLista(cabecaLista); break;
            case 2: removerItemLista(cabecaLista); break;
            case 3: listarItensLista(*cabecaLista); break;
            case 4: buscarSequencialLista(*cabecaLista); break;
            case 0: printf("\nVoltando ao menu principal...\n"); break;
            default: printf("\nOpcao invalida!\n"); break;
        }
    } while (opcao != 0);
}

// --- Implementação das Funções do VETOR ---

void inserirItemVetor(struct Item mochila[], int* numItens) {
    // Esta função é similar à versão anterior, agrupando itens existentes.
    char nomeBusca[30];
    printf("\nNome do item: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            int qtd;
            printf("Item ja existe. Adicionar quantidade: ");
            scanf("%d", &qtd);
            limparBuffer();
            mochila[i].quantidade += qtd;
            printf("Quantidade atualizada!\n");
            return;
        }
    }

    if (*numItens >= MAX_ITENS) {
        printf("Mochila cheia!\n");
        return;
    }

    strcpy(mochila[*numItens].nome, nomeBusca);
    printf("Tipo do item: ");
    fgets(mochila[*numItens].tipo, 20, stdin);
    mochila[*numItens].tipo[strcspn(mochila[*numItens].tipo, "\n")] = 0;
    printf("Quantidade: ");
    scanf("%d", &mochila[*numItens].quantidade);
    limparBuffer();
    (*numItens)++;
    printf("Novo item adicionado!\n");
}

void removerItemVetor(struct Item mochila[], int* numItens) {
    if (*numItens == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    char nomeParaRemover[30];
    printf("\nNome do item a remover: ");
    fgets(nomeParaRemover, 30, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = 0;

    int indiceEncontrado = -1;
    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, nomeParaRemover) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado != -1) {
        for (int i = indiceEncontrado; i < *numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*numItens)--;
        printf("Item removido!\n");
    } else {
        printf("Item nao encontrado.\n");
    }
}

void listarItensVetor(struct Item mochila[], int numItens) {
    printf("\n--- ITENS NA MOCHILA (VETOR) ---\n");
    if (numItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-20s | %-15s | %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
    printf("--------------------------------------------------\n");
}

void ordenarVetor(struct Item mochila[], int numItens) {
    if (numItens < 2) {
        printf("\nNao ha itens suficientes para ordenar.\n");
        return;
    }

    int comparacoes = 0;
    // Algoritmo Bubble Sort para ordenar por nome
    for (int i = 0; i < numItens - 1; i++) {
        for (int j = 0; j < numItens - i - 1; j++) {
            comparacoes++;
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                // Troca os itens de lugar
                struct Item temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
    printf("\nVetor ordenado com sucesso! Total de comparacoes na ordenacao: %d\n", comparacoes);
    listarItensVetor(mochila, numItens);
}

void buscarSequencialVetor(struct Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    char nomeBusca[30];
    printf("\nNome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int comparacoes = 0;
    bool encontrado = false;
    for (int i = 0; i < numItens; i++) {
        comparacoes++; // Conta cada comparação de string
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n--- Item Encontrado (Busca Sequencial) ---\n");
            printf("Nome: %s, Tipo: %s, Qtd: %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        printf("\nItem nao encontrado.\n");
    }
    printf("Total de comparacoes realizadas: %d\n", comparacoes);
}

void buscarBinariaVetor(struct Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    char nomeBusca[30];
    printf("\nNome do item a buscar (binaria): ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int comparacoes = 0;
    bool encontrado = false;
    int esquerda = 0, direita = numItens - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        comparacoes++; // Conta uma comparação por iteração
        int res = strcmp(mochila[meio].nome, nomeBusca);

        if (res == 0) {
            printf("\n--- Item Encontrado (Busca Binaria) ---\n");
            printf("Nome: %s, Tipo: %s, Qtd: %d\n", mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade);
            encontrado = true;
            break;
        }
        if (res < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    if (!encontrado) {
        printf("\nItem nao encontrado.\n");
    }
    printf("Total de comparacoes realizadas: %d\n", comparacoes);
}

// --- Implementação das Funções da LISTA ENCADEADA ---

// A inserção na lista é sempre no início, para simplificar.
void inserirItemLista(No** cabeca) {
    char nomeBusca[30];
    printf("\nNome do item: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    // Procura se o item já existe para agrupar
    No* atual = *cabeca;
    while(atual != NULL) {
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            int qtd;
            printf("Item ja existe. Adicionar quantidade: ");
            scanf("%d", &qtd);
            limparBuffer();
            atual->dados.quantidade += qtd;
            printf("Quantidade atualizada!\n");
            return;
        }
        atual = atual->proximo;
    }

    // Se não encontrou, cria um novo nó
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        return;
    }
    strcpy(novoNo->dados.nome, nomeBusca);
    printf("Tipo do item: ");
    fgets(novoNo->dados.tipo, 20, stdin);
    novoNo->dados.tipo[strcspn(novoNo->dados.tipo, "\n")] = 0;
    printf("Quantidade: ");
    scanf("%d", &novoNo->dados.quantidade);
    limparBuffer();

    novoNo->proximo = *cabeca; // O novo nó aponta para a antiga cabeça
    *cabeca = novoNo;          // A cabeça da lista agora é o novo nó
    printf("Novo item adicionado na lista!\n");
}

void removerItemLista(No** cabeca) {
    if (*cabeca == NULL) {
        printf("\nMochila vazia.\n");
        return;
    }
    char nomeParaRemover[30];
    printf("\nNome do item a remover: ");
    fgets(nomeParaRemover, 30, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = 0;

    No* atual = *cabeca;
    No* anterior = NULL;

    // Procura o nó a ser removido
    while (atual != NULL && strcmp(atual->dados.nome, nomeParaRemover) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Se não encontrou
    if (atual == NULL) {
        printf("Item nao encontrado.\n");
        return;
    }

    // Se é o primeiro nó da lista
    if (anterior == NULL) {
        *cabeca = atual->proximo;
    } else { // Se está no meio ou fim
        anterior->proximo = atual->proximo;
    }
    free(atual); // Libera a memória do nó removido
    printf("Item removido da lista!\n");
}

void listarItensLista(No* cabeca) {
    printf("\n--- ITENS NA MOCHILA (LISTA) ---\n");
    if (cabeca == NULL) {
        printf("A mochila esta vazia.\n");
        return;
    }
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("--------------------------------------------------\n");
    No* atual = cabeca;
    while (atual != NULL) {
        printf("%-20s | %-15s | %d\n", atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
    printf("--------------------------------------------------\n");
}

void buscarSequencialLista(No* cabeca) {
     if (cabeca == NULL) {
        printf("\nMochila vazia.\n");
        return;
    }
    char nomeBusca[30];
    printf("\nNome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int comparacoes = 0;
    bool encontrado = false;
    No* atual = cabeca;
    while (atual != NULL) {
        comparacoes++;
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            printf("\n--- Item Encontrado (Busca Sequencial) ---\n");
            printf("Nome: %s, Tipo: %s, Qtd: %d\n", atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            encontrado = true;
            break;
        }
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("\nItem nao encontrado.\n");
    }
    printf("Total de comparacoes realizadas: %d\n", comparacoes);
}

void liberarLista(No** cabeca) {
    No* atual = *cabeca;
    No* proximoNo;
    while (atual != NULL) {
        proximoNo = atual->proximo;
        free(atual);
        atual = proximoNo;
    }
    *cabeca = NULL;
}


// --- Funções Auxiliares ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}