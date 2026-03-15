#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Sablon {
    int id;
    char* masina;
    char* reparatie;
    float cost;
};

struct Nod {
    struct Sablon data;
    struct Nod* next;
    struct Nod* prev;
};

struct Sablon initializare(int id, const char* masina, const char* reparatie, float cost) {
    struct Sablon s;
    s.id = id;

    s.masina = (char*)malloc(strlen(masina) + 1);
    if (s.masina != NULL)
        strcpy_s(s.masina, strlen(masina) + 1, masina);
    else {
        printf("Eroare alocare masina!\n");
        exit(1);
    }

    s.reparatie = (char*)malloc(strlen(reparatie) + 1);
    if (s.reparatie != NULL)
        strcpy_s(s.reparatie, strlen(reparatie) + 1, reparatie);
    else {
        printf("Eroare alocare reparatie!\n");
        free(s.masina);
        exit(1);
    }

    s.cost = cost;
    return s;
}

void afisareSablon(struct Sablon s) {
    printf("ID:%d Masina:%s Reparatie:%s Cost:%.2f\n", s.id, s.masina, s.reparatie, s.cost);
}

void inserareFinal(struct Nod** head, struct Sablon s) {
    struct Nod* nodNou = (struct Nod*)malloc(sizeof(struct Nod));
    if (nodNou == NULL) {
        printf("Eroare alocare nod!\n");
        exit(1);
    }

    nodNou->data = s;
    nodNou->next = NULL;
    nodNou->prev = NULL;

    if (*head == NULL) {
        *head = nodNou;
        return;
    }

    struct Nod* temp = *head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = nodNou;
    nodNou->prev = temp;
}

void afisareLista(struct Nod* head) {
    while (head != NULL) {
        afisareSablon(head->data);
        head = head->next;
    }
}

void modificaCost(struct Nod* head, int id, float costNou) {
    while (head != NULL) {
        if (head->data.id == id) {
            head->data.cost = costNou;
            return;
        }
        head = head->next;
    }
    printf("ID-ul %d nu a fost gasit!\n", id);
}

void dezalocareLista(struct Nod** head) {
    struct Nod* temp = *head;
    while (temp != NULL) {
        struct Nod* urm = temp->next;
        free(temp->data.masina);
        free(temp->data.reparatie);
        free(temp);
        temp = urm;
    }
    *head = NULL;
}

int main() {
    struct Nod* lista = NULL;

    inserareFinal(&lista, initializare(1, "BMW", "Schimb ulei", 1000));
    inserareFinal(&lista, initializare(2, "Audi", "Frana", 750));
    inserareFinal(&lista, initializare(3, "Volvo", "Ambreiaj", 1200));

    printf("Lista initiala:\n");
    afisareLista(lista);

    printf("\nDupa modificare cost:\n");
    modificaCost(lista, 2, 850);
    afisareLista(lista);

    dezalocareLista(&lista);

    return 0;
}