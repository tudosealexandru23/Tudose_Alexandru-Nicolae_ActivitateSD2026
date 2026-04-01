#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraOspatar {
    int id;
    int varsta;
    float salariu;
    char* restaurant;
    char* nume;
    unsigned char literaContract;
} Ospatar;

typedef struct Nod {
    Ospatar info;
    struct Nod* prev;
    struct Nod* next;
} Nod;

typedef struct ListaDubla {
    Nod* prim;
    Nod* ultim;
} LD;

Ospatar citireOspatarDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Ospatar o;
    aux = strtok(buffer, sep);
    o.id = atoi(aux);
    o.varsta = atoi(strtok(NULL, sep));
    o.salariu = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    o.restaurant = malloc(strlen(aux) + 1);
    strcpy(o.restaurant, aux);
    aux = strtok(NULL, sep);
    o.nume = malloc(strlen(aux) + 1);
    strcpy(o.nume, aux);
    o.literaContract = *strtok(NULL, sep);
    return o;
}

void afisareOspatar(Ospatar o) {
    printf("Id: %d\n", o.id);
    printf("Varsta: %d\n", o.varsta);
    printf("Salariu: %.2f\n", o.salariu);
    printf("Restaurant: %s\n", o.restaurant);
    printf("Nume: %s\n", o.nume);
    printf("Litera Contract: %c\n\n", o.literaContract);
}

void afisareListaOspatariDeLaInceput(LD lista) {
    Nod* p = lista.prim;
    while (p) {
        afisareOspatar(p->info);
        p = p->next;
    }
}

void afisareListaOspatariDeLaSfarsit(LD lista) {
    Nod* p = lista.ultim;
    while (p) {
        afisareOspatar(p->info);
        p = p->prev;
    }
}

void adaugaOspatarInLista(LD* lista, Ospatar oNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = oNou;
    nou->prev = NULL;
    nou->next = NULL;
    if (lista->prim != NULL) {
        nou->prev = lista->ultim;
        lista->ultim->next = nou;
        lista->ultim = nou;
    }
    else {
        lista->prim = nou;
        lista->ultim = nou;
    }
}

void stergeNodPozitie(LD* lista, int pozitie) {
    if (pozitie < 0 || lista->prim == NULL) return;
    Nod* p = lista->prim;
    int index = 0;
    while (p != NULL && index < pozitie) {
        p = p->next;
        index++;
    }
    if (p == NULL) return;
    if (p->prev != NULL) p->prev->next = p->next;
    else lista->prim = p->next;
    if (p->next != NULL) p->next->prev = p->prev;
    else lista->ultim = p->prev;
    free(p->info.nume);
    free(p->info.restaurant);
    free(p);
}

void adaugaSortatDupaSalariu(LD* lista, Ospatar oNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = oNou;
    nou->prev = NULL;
    nou->next = NULL;
    if (lista->prim == NULL) {
        lista->prim = nou;
        lista->ultim = nou;
    }
    else if (oNou.salariu < lista->prim->info.salariu) {
        nou->next = lista->prim;
        lista->prim->prev = nou;
        lista->prim = nou;
    }
    else {
        Nod* p = lista->prim;
        while (p->next != NULL && p->next->info.salariu < oNou.salariu)
            p = p->next;
        nou->next = p->next;
        nou->prev = p;
        if (p->next != NULL) p->next->prev = nou;
        else lista->ultim = nou;
        p->next = nou;
    }
}

int main() {
    LD lista;
    lista.prim = NULL;
    lista.ultim = NULL;
    FILE* f = fopen("Ospatari.txt", "r");
    if (f) {
        while (!feof(f)) {
            Ospatar o = citireOspatarDinFisier(f);
            adaugaOspatarInLista(&lista, o);
        }
        fclose(f);
    }
    printf("Ospatarii din lista (de la inceput):\n");
    afisareListaOspatariDeLaInceput(lista);
    printf("Stergem nodul de pe pozitia 1...\n");
    stergeNodPozitie(&lista, 1);
    printf("Lista dupa ce am sters nodul:\n");
    afisareListaOspatariDeLaInceput(lista);
    return 0;
}