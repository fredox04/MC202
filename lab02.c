#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _ponto {
    int x, y, z;
} Ponto;

typedef struct _labirinto {
    char ***Posicao;
    int nx, ny, nz, num_entradas, num_saidas;
    Ponto *entrada, *saida;
} Labirinto;

Labirinto *LeLabirinto(char *nomearq) {
    int contador_e = 0;
    int contador_s = 0;
    FILE *fp = fopen(nomearq,"r");
    Labirinto *L = (Labirinto *)calloc(1,sizeof(Labirinto));

    fscanf(fp,"%d %d %d %d \n",&L->nx,&L->ny,&L->nz,&L->num_entradas);
    L->Posicao = (char ***)calloc(L->nz,sizeof(char **));

    L->entrada = (Ponto *)calloc(L->num_entradas,sizeof(Ponto));
    L->num_saidas = 0;

    for (int z = 0; z < L->nz; z++) {
        L->Posicao[z] = (char **)calloc(L->ny,sizeof(char *));
        for (int y = 0; y < L->ny; y++) {
            L->Posicao[z][y] = (char *)calloc(L->nx,sizeof(char ));
        }
    }

    for (int z = 0; z < L->nz; z++) {
        for (int y = 0; y < L->ny; y++) {
            for (int x = 0; x < L->nx; x++) {
                fscanf(fp,"%c\t",&L->Posicao[z][y][x]);
                if (L->Posicao[z][y][x] == 'E') {
                    L->entrada[contador_e].x = x;
                    L->entrada[contador_e].y = y;
                    L->entrada[contador_e].z = z;
                    L->Posicao[z][y][x] = 'P';
                    contador_e++;
                } else if (L->Posicao[z][y][x] == 'S') {
                    L->num_saidas++;
                }
            }
            fscanf(fp,"\n");
        }
    }

    L->saida = (Ponto *)calloc(L->num_saidas, sizeof(Ponto));

    contador_s = 0;

    for (int z = 0; z < L->nz; z++) {
        for (int y = 0; y < L->ny; y++) {
            for (int x = 0; x < L->nx; x++) {
                if (L->Posicao[z][y][x] == 'S') {
                    L->saida[contador_s].x = x;
                    L->saida[contador_s].y = y;
                    L->saida[contador_s].z = z;
                    L->Posicao[z][y][x] = 'P';
                    contador_s++;
                }
            }
        }
    }

    fclose(fp);
    return L;
}

void DestroiLabirinto(Labirinto *L) {
    if (L != NULL) {
        for (int z = 0; z < L->nz; z++) {
            for (int y = 0; y < L->ny; y++) {
                free(L->Posicao[z][y]);
            }
            free(L->Posicao[z]);
        }
        free(L->Posicao);
        free(L);
    }
}

bool Backtrack(Labirinto *L, Ponto P) {
    bool res = false;

    for (int i = 0; i < L->num_saidas && !res; i++) {
        if ((P.z == L->saida[i].z) && (P.y == L->saida[i].y) && (P.x == L->saida[i].x)) {
            res = true;
        }
    }

    if (!res) {
        Ponto *C = (Ponto *)calloc(6,sizeof(Ponto));
        C[0].x = P.x - 1; C[0].y = P.y    ; C[0].z = P.z;
        C[1].x = P.x + 1; C[1].y = P.y    ; C[1].z = P.z;
        C[2].x = P.x;     C[2].y = P.y - 1; C[2].z = P.z;
        C[3].x = P.x;     C[3].y = P.y + 1; C[3].z = P.z;
        C[4].x = P.x;     C[4].y = P.y    ; C[4].z = P.z - 1;
        C[5].x = P.x;     C[5].y = P.y    ; C[5].z = P.z + 1;


        for (int i = 0; i < 6 && !res; i++) {
            if ((C[i].x >= 0) && (C[i].x < L->nx) && 
                (C[i].y >= 0) && (C[i].y < L->ny) &&
                (C[i].z >= 0) && (C[i].z < L->nz)) { 
                if (L->Posicao[C[i].z][C[i].y][C[i].x] != 'X') {
                    char valor_correto = L->Posicao[C[i].z][C[i].y][C[i].x];
                    L->Posicao[C[i].z][C[i].y][C[i].x] = 'X'; 
                    res = Backtrack(L,C[i]);
                    L->Posicao[C[i].z][C[i].y][C[i].x] = valor_correto;
                }
            }
        }
        free(C);
    }
    return res;
}

int main(int argc, char **argv) {
    Labirinto *L = NULL;

    L = LeLabirinto(argv[1]);

    for (int i = 0; i < L->num_entradas; i++) {
        if (Backtrack(L,L->entrada[i])) {
            printf("[Entrada %d] Saída encontrada!\n", i);
        }
        else {
            printf("[Entrada %d] Saída não encontrada!\n", i);
        }
    }
    DestroiLabirinto(L);
    return 0;
}
