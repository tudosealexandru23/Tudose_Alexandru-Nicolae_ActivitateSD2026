#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// folosesc fisierul PRODUSE.txt

struct StructuraProdus {
    int id;
    int stoc;
    float pret;
    char* denumire;
    char* categorie;
    unsigned char initialaFurnizor;
};
typedef struct StructuraProdus Produs;

void afisareProdus(Produs p) {
    printf("ID: %d\n", p.id);
    printf("Stoc: %d\n", p.stoc);
    printf("Pret: %.2f\n", p.pret);
    printf("Denumire: %s\n", p.denumire);
    printf("Categorie: %s\n", p.categorie);
    printf("Initiala furnizor: %c\n\n", p.initialaFurnizor);
}

void afisareVectorProduse(Produs* produse, int nrProduse) {
    for (int i = 0; i < nrProduse; i++) {
        afisareProdus(produse[i]);
    }
}

void adaugaProdusInVector(Produs** produse, int* nrProduse, Produs produsNou) {
    Produs* aux = (Produs*)malloc(sizeof(Produs) * ((*nrProduse) + 1));

    for (int i = 0; i < *nrProduse; i++) {
        aux[i] = (*produse)[i];
    }

    aux[*nrProduse] = produsNou;

    free(*produse);
    *produse = aux;
    (*nrProduse)++;
}

Produs citireProdusFisier(FILE* file) {
    Produs p;
    char buffer[100];
    char sep[4] = ",;\n";

    fgets(buffer, 100, file);

    p.id = atoi(strtok(buffer, sep));
    p.stoc = atoi(strtok(NULL, sep));
    p.pret = atof(strtok(NULL, sep));

    char* aux;

    aux = strtok(NULL, sep);
    p.denumire = (char*)malloc(strlen(aux) + 1);
    strcpy(p.denumire, aux);

    aux = strtok(NULL, sep);
    p.categorie = (char*)malloc(strlen(aux) + 1);
    strcpy(p.categorie, aux);

    p.initialaFurnizor = strtok(NULL, sep)[0];

    return p;
}

Produs* citireVectorProduseFisier(const char* numeFisier, int* nrProduse) {
    FILE* f = fopen(numeFisier, "r");
    Produs* produse = NULL;
    *nrProduse = 0;

    while (!feof(f)) {
        Produs p = citireProdusFisier(f);
        if (p.denumire != NULL) { // evit? ultima citire invalid?
            adaugaProdusInVector(&produse, nrProduse, p);
        }
    }

    fclose(f);
    return produse;
}

void dezalocareVectorProduse(Produs** produse, int* nrProduse) {
    for (int i = 0; i < *nrProduse; i++) {
        free((*produse)[i].denumire);
        free((*produse)[i].categorie);
    }

    free(*produse);
    *produse = NULL;
    *nrProduse = 0;
}

int main() {
    int nrProduse = 0;

    Produs* produse = citireVectorProduseFisier("PRODUSE.txt", &nrProduse);

    afisareVectorProduse(produse, nrProduse);

    dezalocareVectorProduse(&produse, &nrProduse);

    return 0;
}