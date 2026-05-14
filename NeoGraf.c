#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

typedef struct Oras {
    int id;
    char nume[30];
} Oras;

void adaugaDrum(int matrice[MAX][MAX], int oras1, int oras2) {
    matrice[oras1][oras2] = 1;
    matrice[oras2][oras1] = 1;
}

void afisareOrase(Oras orase[], int nrOrase) {
    printf("Lista oraselor:\n");

    for (int i = 0; i < nrOrase; i++) {
        printf("%d. %s\n", orase[i].id, orase[i].nume);
    }
}

void afisareMatrice(int matrice[MAX][MAX], int nrOrase) {
    printf("\nMatricea de adiacenta:\n\n");

    for (int i = 0; i < nrOrase; i++) {
        for (int j = 0; j < nrOrase; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}

void afisareDrumuri(int matrice[MAX][MAX], Oras orase[], int nrOrase) {
    printf("\nDrumuri existente:\n");

    for (int i = 0; i < nrOrase; i++) {
        for (int j = i + 1; j < nrOrase; j++) {
            if (matrice[i][j] == 1) {
                printf("%s - %s\n", orase[i].nume, orase[j].nume);
            }
        }
    }
}

void veciniOras(int matrice[MAX][MAX], Oras orase[], int nrOrase, int idOras) {
    int gasit = 0;

    printf("\nOrase conectate direct cu %s:\n", orase[idOras].nume);

    for (int i = 0; i < nrOrase; i++) {
        if (matrice[idOras][i] == 1) {
            printf("%s\n", orase[i].nume);
            gasit = 1;
        }
    }

    if (gasit == 0) {
        printf("Nu exista drumuri directe.\n");
    }
}

int gradOras(int matrice[MAX][MAX], int nrOrase, int idOras) {
    int grad = 0;

    for (int i = 0; i < nrOrase; i++) {
        if (matrice[idOras][i] == 1) {
            grad++;
        }
    }

    return grad;
}

void BFS(int matrice[MAX][MAX], Oras orase[], int nrOrase, int start) {
    int vizitat[MAX] = { 0 };
    int coada[MAX];
    int inceput = 0;
    int sfarsit = 0;

    vizitat[start] = 1;
    coada[sfarsit++] = start;

    printf("\nParcurgere BFS de la %s:\n", orase[start].nume);

    while (inceput < sfarsit) {
        int nod = coada[inceput++];
        printf("%s ", orase[nod].nume);

        for (int i = 0; i < nrOrase; i++) {
            if (matrice[nod][i] == 1 && vizitat[i] == 0) {
                vizitat[i] = 1;
                coada[sfarsit++] = i;
            }
        }
    }

    printf("\n");
}

void DFS(int matrice[MAX][MAX], Oras orase[], int nrOrase, int start, int vizitat[MAX]) {
    vizitat[start] = 1;
    printf("%s ", orase[start].nume);

    for (int i = 0; i < nrOrase; i++) {
        if (matrice[start][i] == 1 && vizitat[i] == 0) {
            DFS(matrice, orase, nrOrase, i, vizitat);
        }
    }
}

int main() {
    int nrOrase = 6;

    Oras orase[MAX] = {
        {0, "Brasov"},
        {1, "Sibiu"},
        {2, "Cluj"},
        {3, "Bucuresti"},
        {4, "Pitesti"},
        {5, "Constanta"}
    };

    int matrice[MAX][MAX] = { 0 };

    adaugaDrum(matrice, 0, 1);
    adaugaDrum(matrice, 0, 3);
    adaugaDrum(matrice, 1, 2);
    adaugaDrum(matrice, 3, 4);
    adaugaDrum(matrice, 4, 5);
    adaugaDrum(matrice, 2, 5);

    afisareOrase(orase, nrOrase);
    afisareMatrice(matrice, nrOrase);
    afisareDrumuri(matrice, orase, nrOrase);

    veciniOras(matrice, orase, nrOrase, 0);

    printf("\nGradul orasului %s este: %d\n", orase[0].nume, gradOras(matrice, nrOrase, 0));

    BFS(matrice, orase, nrOrase, 0);

    int vizitat[MAX] = { 0 };

    printf("\nParcurgere DFS de la %s:\n", orase[0].nume);
    DFS(matrice, orase, nrOrase, 0, vizitat);
    printf("\n");

    return 0;
}