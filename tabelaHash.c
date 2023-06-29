#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 151
#define MAX_tam 10
#define MIN_tam 5

typedef struct NoLista {
    struct NoLista* proximo;
    int valor;
} NoLista;

typedef struct Lista {
    NoLista* primeiro;
    NoLista* ultimo;
    int quantidade;
} Lista;

typedef struct NoHash {
    struct NoHash* proximo;
    char* valor;
} NoHash;

typedef struct TabelaHash {
    Lista vetores[M];
} TabelaHash;

int hash(const char* chave) {
    int soma = 0;
    int tamanho = strlen(chave);
    for (int i = 0; i < tamanho; i++) {
        soma += chave[i];
    }
    return soma % M;
}

void inicializarLista(Lista* l) {
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->quantidade = 0;
}

void inserirInicio(Lista* l, int valor) {
    NoLista* n = malloc(sizeof(NoLista));
    n->valor = valor;
    n->proximo = l->primeiro;
    l->primeiro = n;
    if (l->ultimo == NULL) l->ultimo = n;
    l->quantidade++;
}

void inserirFim(Lista* l, int valor) {
    NoLista* n = malloc(sizeof(NoLista));
    n->valor = valor;
    n->proximo = NULL;
    if (l->ultimo == NULL) {
        l->primeiro = n;
        l->ultimo = n;
    } else {
        l->ultimo->proximo = n;
        l->ultimo = n;
    }
    l->quantidade++;
}

int removerPrimeiro(Lista* l) {
    if (l->primeiro == NULL) return 0;
    NoLista* aux = l->primeiro;
    l->primeiro = aux->proximo;
    if (l->primeiro == NULL) l->ultimo = NULL;
    l->quantidade--;
    int valor = aux->valor;
    free(aux);
    return valor;
}

int removerUltimo(Lista* l) {
    if (l->ultimo == NULL) return 0;
    NoLista* aux = l->primeiro;
    NoLista* anterior = NULL;
    while (aux->proximo != NULL) {
        anterior = aux;
        aux = aux->proximo;
    }
    if (anterior == NULL) {
        l->primeiro = NULL;
        l->ultimo = NULL;
    } else {
        anterior->proximo = NULL;
        l->ultimo = anterior;
    }
    l->quantidade--;
    int valor = aux->valor;
    free(aux);
    return valor;
}

int remover(Lista* l, int posicao) {
    if ((posicao < 0) || (posicao >= l->quantidade)) return 0;
    if (posicao == 0) return removerPrimeiro(l);
    else if (posicao == l->quantidade - 1) return removerUltimo(l);
    else {
        NoLista* aux = l->primeiro;
        NoLista* anterior = NULL;
        for (int i = 0; i < posicao; i++) {
            anterior = aux;
            aux = aux->proximo;
        }
        anterior->proximo = aux->proximo;
        int valor = aux->valor;
        free(aux);
        l->quantidade--;
        return valor;
    }
}

void inicializarTabelaHash(TabelaHash* tabelaHash) {
    for (int i = 0; i < M; i++) {
        inicializarLista(&(tabelaHash->vetores[i]));
    }
}

void inserirTabelaHash(TabelaHash* tabelaHash, const char* chave) {
    int indice = hash(chave);
    inserirInicio(&(tabelaHash->vetores[indice]), atoi(chave));
}

int buscarTabelaHash(const TabelaHash* tabelaHash, const char* chave) {
    int indice = hash(chave);
    const Lista* lista = &(tabelaHash->vetores[indice]);
    NoLista* aux = lista->primeiro;
    while (aux != NULL) {
        if (aux->valor == atoi(chave)) {
            return 1;
        }
        aux = aux->proximo;
    }
    return 0;
}

void removerTabelaHash(TabelaHash* tabelaHash, const char* chave) {
    int indice = hash(chave);
    Lista* lista = &(tabelaHash->vetores[indice]);
    NoLista* aux = lista->primeiro;
    NoLista* anterior = NULL;

    while (aux != NULL) {
        if (aux->valor == atoi(chave)) {
            if (anterior == NULL) {
                lista->primeiro = aux->proximo;
            } else {
                anterior->proximo = aux->proximo;
            }

            if (aux == lista->ultimo) {
                lista->ultimo = anterior;
            }

            free(aux);
            lista->quantidade--;
            return;
        }

        anterior = aux;
        aux = aux->proximo;
    }
}


int main() {
    TabelaHash tabelaHash;
    inicializarTabelaHash(&tabelaHash);

    srand(time(NULL));
    int g = 0;
    int quantidadeDePalavras;
    int tam;
    char palavra[MAX_tam + 1];
    
    printf("Insira a quantidade de palavras para serem geradas aleatoriamente: ");
    scanf("%d", &quantidadeDePalavras);

    char** vetorPalavras = malloc(quantidadeDePalavras * sizeof(char*));
    if (vetorPalavras == NULL) {
        printf("Erro ao alocar memória para o vetor de palavras.\n");
        return 1;
    }
    
    while (g < quantidadeDePalavras) {
        tam = rand() % (MAX_tam - MIN_tam + 1) + MIN_tam;
        for (int i = 0; i < tam; i++) {
            palavra[i] = 'a' + rand() % 26;
        }
        palavra[tam] = '\0';
        g++;
        
        //printf("%s\n", palavra);
        
        vetorPalavras[g - 1] = malloc((tam + 1) * sizeof(char));
        if (vetorPalavras[g - 1] == NULL) {
            printf("Erro ao alocar memória para a palavra.\n");
            return 1;
        }
        
        strcpy(vetorPalavras[g - 1], palavra);
        inserirTabelaHash(&tabelaHash, palavra);
    }
    
    //double tempo = 0.0;
    //clock_t begin = clock();
    //buscarTabelaHash(&tabelaHash, vetorPalavras[9]);
    //clock_t end = clock();
    //tempo += (double) (end-begin);
    //printf("%f\n", tempo);
        
    for (int i = 0; i < quantidadeDePalavras; i++) {
        free(vetorPalavras[i]);
    }
    free(vetorPalavras);

    //Verificar e imprimir a quantidade de valores em cada lista de cada vetor da tabela hash
    for (int j = 0; j < M; j++) {
        printf("Vetor %d: ", j);
        Lista* lista = &tabelaHash.vetores[j];
        NoLista* aux = lista->primeiro;
        printf("\n");
        printf("Quantidade de valores: %d\n", lista->quantidade);
        //printf("%d\n", lista->quantidade);
    }
    return 0;
}