#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// --- Constantes e Estruturas ---
#define MAX_COMPONENTES 20

// Struct para os componentes da torre de fuga
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // Prioridade de 1 a 10
} Componente;

// --- Protótipos das Funções ---

// Funções de Gerenciamento
void adicionarComponente(Componente torre[], int* numComponentes);
void mostrarComponentes(Componente torre[], int numComponentes);

// Algoritmos de Ordenação
long long bubbleSortNome(Componente torre[], int n);
long long insertionSortTipo(Componente torre[], int n);
long long selectionSortPrioridade(Componente torre[], int n);

// Busca
void buscaBinariaPorNome(Componente torre[], int n);

// Análise de Desempenho
void medirTempoEComparacoes(long long (*algoritmo)(Componente[], int), Componente torre[], int n, const char* nomeAlgoritmo);

// Auxiliares
void limparBuffer();
void limparTela();

// --- Função Principal ---
int main() {
    Componente torreDeFuga[MAX_COMPONENTES];
    int numComponentes = 0;
    bool ordenadoPorNome = false; // Controle para a busca binária
    int opcao;

    do {
        printf("\n========= PAINEL DE MONTAGEM DA TORRE DE FUGA =========\n");
        printf("Componentes catalogados: %d/%d\n", numComponentes, MAX_COMPONENTES);
        printf("Status: Lista %s ordenada por nome.\n", (ordenadoPorNome ? "ESTA" : "NAO ESTA"));
        printf("---------------------------------------------------------\n");
        printf("1. Adicionar Componente\n");
        printf("2. Listar Componentes Atuais\n");
        printf("\n--- ESTRATEGIAS DE ORDENACAO ---\n");
        printf("3. Ordenar por NOME (Bubble Sort)\n");
        printf("4. Ordenar por TIPO (Insertion Sort)\n");
        printf("5. Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("\n--- ACOES FINAIS ---\n");
        printf("6. Buscar Componente-Chave (Busca Binaria por Nome)\n");
        printf("0. Cancelar Missao (Sair)\n");
        printf("=========================================================\n");
        printf("Escolha sua acao: ");

        char input[10];
        fgets(input, 10, stdin);
        if (sscanf(input, "%d", &opcao) != 1) {
            opcao = -1;
        }

        // Se uma ordenação diferente de "por nome" for escolhida, a flag é desmarcada.
        if (opcao == 4 || opcao == 5) {
            ordenadoPorNome = false;
        }

        switch (opcao) {
            case 1:
                adicionarComponente(torreDeFuga, &numComponentes);
                ordenadoPorNome = false; // Adicionar um novo item desordena a lista
                break;
            case 2:
                mostrarComponentes(torreDeFuga, numComponentes);
                break;
            case 3:
                medirTempoEComparacoes(bubbleSortNome, torreDeFuga, numComponentes, "Bubble Sort por Nome");
                ordenadoPorNome = true; // Marca que a lista está ordenada por nome
                break;
            case 4:
                medirTempoEComparacoes(insertionSortTipo, torreDeFuga, numComponentes, "Insertion Sort por Tipo");
                break;
            case 5:
                medirTempoEComparacoes(selectionSortPrioridade, torreDeFuga, numComponentes, "Selection Sort por Prioridade");
                break;
            case 6:
                if (ordenadoPorNome) {
                    buscaBinariaPorNome(torreDeFuga, numComponentes);
                } else {
                    printf("\n[ALERTA] A busca binaria so funciona se a lista estiver ordenada por nome!\n");
                    printf("Por favor, use a opcao 3 para ordenar antes de buscar.\n");
                }
                break;
            case 0:
                printf("\nMissao cancelada. A ilha te consumiu...\n");
                break;
            default:
                printf("\nComando invalido! Foco, sobrevivente!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}


// --- Implementação das Funções ---

/**
 * @brief Adiciona um novo componente à lista da torre.
 */
void adicionarComponente(Componente torre[], int* numComponentes) {
    if (*numComponentes >= MAX_COMPONENTES) {
        printf("\nCapacidade maxima de componentes atingida!\n");
        return;
    }

    Componente* novo = &torre[*numComponentes];
    printf("\n--- Adicionando novo componente ---\n");
    printf("Nome (ex: Chip Central): ");
    fgets(novo->nome, 30, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = 0;

    printf("Tipo (ex: Controle, Suporte, Propulsao): ");
    fgets(novo->tipo, 20, stdin);
    novo->tipo[strcspn(novo->tipo, "\n")] = 0;

    printf("Prioridade (1 a 10): ");
    scanf("%d", &novo->prioridade);
    limparBuffer();

    (*numComponentes)++;
    printf("Componente '%s' adicionado com sucesso!\n", novo->nome);
}

/**
 * @brief Exibe todos os componentes em uma tabela formatada.
 */
void mostrarComponentes(Componente torre[], int numComponentes) {
    printf("\n---------- LISTA DE COMPONENTES PARA A TORRE ----------\n");
    if (numComponentes == 0) {
        printf("Nenhum componente catalogado ainda.\n");
        return;
    }
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < numComponentes; i++) {
        printf("%-20s | %-15s | %d\n", torre[i].nome, torre[i].tipo, torre[i].prioridade);
    }
    printf("------------------------------------------------------\n");
}


/**
 * @brief Ordena os componentes por NOME usando Bubble Sort.
 * @return O número de comparações realizadas.
 */
long long bubbleSortNome(Componente torre[], int n) {
    long long comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcmp(torre[j].nome, torre[j + 1].nome) > 0) {
                Componente temp = torre[j];
                torre[j] = torre[j + 1];
                torre[j + 1] = temp;
            }
        }
    }
    return comparacoes;
}

/**
 * @brief Ordena os componentes por TIPO usando Insertion Sort.
 * @return O número de comparações realizadas.
 */
long long insertionSortTipo(Componente torre[], int n) {
    long long comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = torre[i];
        int j = i - 1;
        while (j >= 0 && (comparacoes++, strcmp(torre[j].tipo, chave.tipo) > 0)) {
            torre[j + 1] = torre[j];
            j = j - 1;
        }
        torre[j + 1] = chave;
    }
    return comparacoes;
}

/**
 * @brief Ordena os componentes por PRIORIDADE (decrescente) usando Selection Sort.
 * @return O número de comparações realizadas.
 */
long long selectionSortPrioridade(Componente torre[], int n) {
    long long comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            // Ordena da maior para a menor prioridade
            if (torre[j].prioridade > torre[max_idx].prioridade) {
                max_idx = j;
            }
        }
        Componente temp = torre[max_idx];
        torre[max_idx] = torre[i];
        torre[i] = temp;
    }
    return comparacoes;
}

/**
 * @brief Realiza uma busca binária por um componente pelo nome.
 */
void buscaBinariaPorNome(Componente torre[], int n) {
    char nomeBusca[30];
    printf("\nDigite o nome do componente-chave a buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int esquerda = 0, direita = n - 1;
    bool encontrado = false;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int res = strcmp(torre[meio].nome, nomeBusca);

        if (res == 0) {
            printf("\n[SUCESSO] Componente-chave '%s' encontrado!\n", nomeBusca);
            printf("Detalhes: Tipo '%s', Prioridade %d\n", torre[meio].tipo, torre[meio].prioridade);
            printf("A torre de fuga pode ser ativada!\n");
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
        printf("\n[FALHA] Componente-chave '%s' nao encontrado. A fuga e impossivel!\n", nomeBusca);
    }
}


/**
 * @brief Função 'invólucro' para medir o tempo e as comparações de um algoritmo de ordenação.
 * @param algoritmo Ponteiro para a função de ordenação a ser executada.
 * @param torre O vetor de componentes.
 * @param n O número de componentes.
 * @param nomeAlgoritmo Uma string com o nome do algoritmo para exibição.
 */
void medirTempoEComparacoes(long long (*algoritmo)(Componente[], int), Componente torre[], int n, const char* nomeAlgoritmo) {
    if (n == 0) {
        printf("\nNao ha componentes para ordenar.\n");
        return;
    }

    // Cria uma cópia para não alterar o original desnecessariamente se não for o objetivo
    Componente copiaTorre[MAX_COMPONENTES];
    memcpy(copiaTorre, torre, n * sizeof(Componente));

    printf("\nIniciando ordenacao com %s...\n", nomeAlgoritmo);

    clock_t inicio = clock(); // Marca o tempo de início
    long long comparacoes = algoritmo(copiaTorre, n); // Executa o algoritmo
    clock_t fim = clock();    // Marca o tempo de fim

    // Calcula o tempo decorrido em milissegundos
    double tempoGasto = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

    // Copia o resultado ordenado de volta para o vetor original
    memcpy(torre, copiaTorre, n * sizeof(Componente));

    printf("Ordenacao concluida!\n");
    printf("\n---------- RELATORIO DE DESEMPENHO ----------\n");
    printf("Algoritmo: %s\n", nomeAlgoritmo);
    printf("Numero de comparacoes: %lld\n", comparacoes);
    printf("Tempo de execucao: %.4f milissegundos\n", tempoGasto);
    printf("---------------------------------------------\n");

    // Mostra o resultado
    mostrarComponentes(torre, n);
}

// --- Funções Auxiliares ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}