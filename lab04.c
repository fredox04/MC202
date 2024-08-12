#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char valor;
    struct No *esquerda, *direita;
} No;

No* novoNo(char valor) {
    No* no = (No*)malloc(sizeof(No));
    no->valor = valor;
    no->esquerda = no->direita = NULL;
    return no;
}

int buscar(char* arr, int inicio, int fim, char valor) {
    int i;
    for (i = inicio; i <= fim; i++) {
        if (arr[i] == valor)
            return i;
    }
    return -1;
}

No* construirArvore(char* preOrdem, char* inOrdem, int inicio, int fim) {
    static int indicePreOrdem = 0;

    if (inicio > fim)
        return NULL;

    No* no = novoNo(preOrdem[indicePreOrdem++]);

    if (inicio == fim)
        return no;

    int indiceInOrdem = buscar(inOrdem, inicio, fim, no->valor);

    no->esquerda = construirArvore(preOrdem, inOrdem, inicio, indiceInOrdem - 1);
    no->direita = construirArvore(preOrdem, inOrdem, indiceInOrdem + 1, fim);

    return no;
}

// Função para calcular a altura da árvore
int alturaArvore(No* raiz) {
    if (raiz == NULL)
        return 0;

    int alturaEsquerda = alturaArvore(raiz->esquerda);
    int alturaDireita = alturaArvore(raiz->direita);

    return (alturaEsquerda > alturaDireita) ? (alturaEsquerda + 1) : (alturaDireita + 1);
}

void imprimirGrauNos(No* raiz) {
    if (raiz == NULL)
        return;

    imprimirGrauNos(raiz->esquerda);

    int grau = 0;
    if (raiz->esquerda != NULL) grau++;
    if (raiz->direita != NULL) grau++;
    printf("%c: %d\n", raiz->valor, grau);

    imprimirGrauNos(raiz->direita);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_pre_ordem> <arquivo_in_ordem>\n", argv[0]);
        return 1;
    }

    FILE* arquivoPreOrdem = fopen(argv[1], "r");
    FILE* arquivoInOrdem = fopen(argv[2], "r");

    if (arquivoPreOrdem == NULL || arquivoInOrdem == NULL) {
        fprintf(stderr, "Erro ao abrir arquivos.\n");
        return 1;
    }

    int tamanho;
    fscanf(arquivoPreOrdem, "%d\n", &tamanho);
    fscanf(arquivoInOrdem, "%d\n", &tamanho);

    char preOrdem[tamanho + 1];
    char inOrdem[tamanho + 1];

    fgets(preOrdem, tamanho + 1, arquivoPreOrdem);
    fgets(inOrdem, tamanho + 1, arquivoInOrdem);

    fclose(arquivoPreOrdem);
    fclose(arquivoInOrdem);

    No* raiz = construirArvore(preOrdem, inOrdem, 0, tamanho - 1);

    printf("Altura da arvore: %d\n", alturaArvore(raiz));
    printf("Grau de cada nó em percurso simétrico:\n");
    imprimirGrauNos(raiz);

    return 0;
}
