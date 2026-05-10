#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Colet {
    int awb;
    char destinatar[50];
    float greutate;
} Colet;

typedef struct Nod {
    Colet info;
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

int factorEchilibru(Nod* rad) {
    if (rad == NULL) {
        return 0;
    }
    return inaltime(rad->stanga) - inaltime(rad->dreapta);
}

Nod* creareNod(Colet c) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
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

Nod* inserare(Nod* rad, Colet c) {
    if (rad == NULL) {
        return creareNod(c);
    }

    if (c.awb < rad->info.awb) {
        rad->stanga = inserare(rad->stanga, c);
    }
    else if (c.awb > rad->info.awb) {
        rad->dreapta = inserare(rad->dreapta, c);
    }
    else {
        return rad;
    }

    rad->inaltime = 1 + maxim(inaltime(rad->stanga), inaltime(rad->dreapta));

    int echilibru = factorEchilibru(rad);

    if (echilibru > 1 && c.awb < rad->stanga->info.awb) {
        return rotireDreapta(rad);
    }

    if (echilibru < -1 && c.awb > rad->dreapta->info.awb) {
        return rotireStanga(rad);
    }

    if (echilibru > 1 && c.awb > rad->stanga->info.awb) {
        rad->stanga = rotireStanga(rad->stanga);
        return rotireDreapta(rad);
    }

    if (echilibru < -1 && c.awb < rad->dreapta->info.awb) {
        rad->dreapta = rotireDreapta(rad->dreapta);
        return rotireStanga(rad);
    }

    return rad;
}

void afisareInordine(Nod* rad) {
    if (rad != NULL) {
        afisareInordine(rad->stanga);
        printf("AWB: %d | Destinatar: %s | Greutate: %.2f kg | Inaltime nod: %d\n",
            rad->info.awb, rad->info.destinatar, rad->info.greutate, rad->inaltime);
        afisareInordine(rad->dreapta);
    }
}

void cautareColet(Nod* rad, int awb) {
    if (rad == NULL) {
        printf("Coletul cu AWB %d nu a fost gasit.\n", awb);
        return;
    }

    if (awb == rad->info.awb) {
        printf("Colet gasit: %s, %.2f kg\n", rad->info.destinatar, rad->info.greutate);
    }
    else if (awb < rad->info.awb) {
        cautareColet(rad->stanga, awb);
    }
    else {
        cautareColet(rad->dreapta, awb);
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

    Colet c1 = { 4312, "Mihai Tudose", 2.40f };
    Colet c2 = { 1287, "Andrei Stoica", 1.15f };
    Colet c3 = { 7860, "Raluca Marin", 4.80f };
    Colet c4 = { 2455, "Danut Ilie", 0.95f };
    Colet c5 = { 9901, "Cristina Pavel", 3.30f };
    Colet c6 = { 6704, "Vlad Barbu", 5.10f };
    Colet c7 = { 3120, "Ioana Dobre", 2.75f };

    radacina = inserare(radacina, c1);
    radacina = inserare(radacina, c2);
    radacina = inserare(radacina, c3);
    radacina = inserare(radacina, c4);
    radacina = inserare(radacina, c5);
    radacina = inserare(radacina, c6);
    radacina = inserare(radacina, c7);

    printf("Colete afisate in ordine dupa AWB:\n\n");
    afisareInordine(radacina);

    printf("\nCautare colet:\n");
    cautareColet(radacina, 6704);

    dezalocare(radacina);

    return 0;
}
