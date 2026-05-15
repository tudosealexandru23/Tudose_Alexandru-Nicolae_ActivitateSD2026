#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pacient {
    int cod;
    char nume[50];
    int varsta;
} Pacient;

typedef struct Nod {
    Pacient info;
    struct Nod* stanga;
    struct Nod* dreapta;
    int inaltime;
} Nod;

int maxim(int a, int b) {
    return a > b ? a : b;
}

int inaltime(Nod* rad) {
    if (rad == NULL) {
        return 0;
    }

    return rad->inaltime;
}

int gradEchilibru(Nod* rad) {
    if (rad == NULL) {
        return 0;
    }

    return inaltime(rad->stanga) - inaltime(rad->dreapta);
}

Nod* creareNod(Pacient p) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));

    nou->info = p;
    nou->stanga = NULL;
    nou->dreapta = NULL;
    nou->inaltime = 1;

    return nou;
}

Nod* rotireDreapta(Nod* y) {

    Nod* x = y->stanga;
    Nod* t2 = x->dreapta;

    x->dreapta = y;
    y->stanga = t2;

    y->inaltime = maxim(inaltime(y->stanga), inaltime(y->dreapta)) + 1;
    x->inaltime = maxim(inaltime(x->stanga), inaltime(x->dreapta)) + 1;

    return x;
}

Nod* rotireStanga(Nod* x) {

    Nod* y = x->dreapta;
    Nod* t2 = y->stanga;

    y->stanga = x;
    x->dreapta = t2;

    x->inaltime = maxim(inaltime(x->stanga), inaltime(x->dreapta)) + 1;
    y->inaltime = maxim(inaltime(y->stanga), inaltime(y->dreapta)) + 1;

    return y;
}

Nod* inserare(Nod* rad, Pacient p) {

    if (rad == NULL) {
        return creareNod(p);
    }

    if (p.cod < rad->info.cod) {
        rad->stanga = inserare(rad->stanga, p);
    }
    else if (p.cod > rad->info.cod) {
        rad->dreapta = inserare(rad->dreapta, p);
    }
    else {
        return rad;
    }

    rad->inaltime = 1 + maxim(inaltime(rad->stanga), inaltime(rad->dreapta));

    int echilibru = gradEchilibru(rad);

    if (echilibru > 1 && p.cod < rad->stanga->info.cod) {
        return rotireDreapta(rad);
    }

    if (echilibru < -1 && p.cod > rad->dreapta->info.cod) {
        return rotireStanga(rad);
    }

    if (echilibru > 1 && p.cod > rad->stanga->info.cod) {
        rad->stanga = rotireStanga(rad->stanga);
        return rotireDreapta(rad);
    }

    if (echilibru < -1 && p.cod < rad->dreapta->info.cod) {
        rad->dreapta = rotireDreapta(rad->dreapta);
        return rotireStanga(rad);
    }

    return rad;
}

void afisareInordine(Nod* rad) {

    if (rad != NULL) {

        afisareInordine(rad->stanga);

        printf("Cod: %d | Nume: %s | Varsta: %d\n",
            rad->info.cod,
            rad->info.nume,
            rad->info.varsta);

        afisareInordine(rad->dreapta);
    }
}

void cautaPacient(Nod* rad, int cod) {

    if (rad == NULL) {
        printf("Pacientul nu exista.\n");
        return;
    }

    if (cod == rad->info.cod) {

        printf("\nPacient gasit:\n");
        printf("Nume: %s\n", rad->info.nume);
        printf("Varsta: %d\n", rad->info.varsta);

    }
    else if (cod < rad->info.cod) {
        cautaPacient(rad->stanga, cod);
    }
    else {
        cautaPacient(rad->dreapta, cod);
    }
}

int numarPacienti(Nod* rad) {

    if (rad == NULL) {
        return 0;
    }

    return 1 + numarPacienti(rad->stanga) + numarPacienti(rad->dreapta);
}

void afisareFrunze(Nod* rad) {

    if (rad != NULL) {

        if (rad->stanga == NULL && rad->dreapta == NULL) {
            printf("%s\n", rad->info.nume);
        }

        afisareFrunze(rad->stanga);
        afisareFrunze(rad->dreapta);
    }
}

void dezalocare(Nod* rad) {

    if (rad != NULL) {

        dezalocare(rad->stanga);
        dezalocare(rad->dreapta);

        free(rad);
    }
}

int main() {

    Nod* radacina = NULL;

    Pacient p1 = { 410, "Marin Andrei", 28 };
    Pacient p2 = { 120, "Popa Cristina", 34 };
    Pacient p3 = { 760, "Dobre Raul", 45 };
    Pacient p4 = { 90, "Ionescu Bianca", 22 };
    Pacient p5 = { 300, "Matei Victor", 39 };
    Pacient p6 = { 890, "Neagu Alex", 31 };
    Pacient p7 = { 250, "Stan Larisa", 27 };

    radacina = inserare(radacina, p1);
    radacina = inserare(radacina, p2);
    radacina = inserare(radacina, p3);
    radacina = inserare(radacina, p4);
    radacina = inserare(radacina, p5);
    radacina = inserare(radacina, p6);
    radacina = inserare(radacina, p7);

    printf("Lista pacientilor:\n\n");
    afisareInordine(radacina);

    cautaPacient(radacina, 300);

    printf("\nNumar total pacienti: %d\n", numarPacienti(radacina));

    printf("\nPacienti aflati in frunzele arborelui:\n");
    afisareFrunze(radacina);

    dezalocare(radacina);

    return 0;
}