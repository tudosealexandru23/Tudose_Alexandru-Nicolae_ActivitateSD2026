#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraOspatar {
	int id;
	int aniExperienta;
	float salariu;
	char* restaurant;
	char* numeOspatar;
	unsigned char nivelCertificare;
};
typedef struct StructuraOspatar Ospatar;

struct Nod {
	Ospatar ospatar;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** tabela;
};
typedef struct HashTable HashTable;

Ospatar citireOspatarDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Ospatar o;
	aux = strtok(buffer, sep);
	o.id = atoi(aux);
	o.aniExperienta = atoi(strtok(NULL, sep));
	o.salariu = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	o.restaurant = malloc(strlen(aux) + 1);
	strcpy(o.restaurant, aux);

	aux = strtok(NULL, sep);
	o.numeOspatar = malloc(strlen(aux) + 1);
	strcpy(o.numeOspatar, aux);

	o.nivelCertificare = *strtok(NULL, sep);
	return o;
}

void afisareOspatar(Ospatar o) {
	printf("Id: %d\n", o.id);
	printf("Ani experienta: %d\n", o.aniExperienta);
	printf("Salariu: %.2f\n", o.salariu);
	printf("Restaurant: %s\n", o.restaurant);
	printf("Nume ospatar: %s\n", o.numeOspatar);
	printf("Nivel certificare: %c\n\n", o.nivelCertificare);
}

void afisareListaOspatari(Nod* cap) {
	while (cap) {
		afisareOspatar(cap->ospatar);
		cap = cap->next;
	}
}

void adaugaOspatarInLista(Nod* cap, Ospatar o) {
	Nod* p = cap;
	while (p->next) {
		p = p->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->ospatar = o;
	nou->next = NULL;
	p->next = nou;
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* nume, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(nume); i++) {
		suma += nume[i];
	}
	return suma % dimensiune;
}

void inserareOspatarInTabela(HashTable hash, Ospatar o) {
	int pozitie = calculeazaHash(o.numeOspatar, hash.dim);
	if (hash.tabela[pozitie] == NULL) {
		hash.tabela[pozitie] = (Nod*)malloc(sizeof(Nod));
		hash.tabela[pozitie]->ospatar = o;
		hash.tabela[pozitie]->next = NULL;
	}
	else {
		adaugaOspatarInLista(hash.tabela[pozitie], o);
	}
}

HashTable citireOspatariDinFisier(const char* numeFisier, int dimensiune) {
	HashTable hash = initializareHashTable(dimensiune);
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		perror("Eroare la deschiderea fisierului");
		return hash;
	}
	while (!feof(f)) {
		Ospatar o = citireOspatarDinFisier(f);
		inserareOspatarInTabela(hash, o);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeOspatari(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			printf("\nOspatarii de pe pozitia %d sunt:\n", i);
			afisareListaOspatari(ht.tabela[i]);
		}
		else {
			printf("\nPe pozitia %d nu avem ospatari\n", i);
		}
	}
}

void dezalocareListaOspatari(Nod** cap) {
	Nod* p = *cap;
	while (p) {
		Nod* temp = p;
		p = p->next;
		if (temp->ospatar.restaurant != NULL) {
			free(temp->ospatar.restaurant);
		}
		if (temp->ospatar.numeOspatar != NULL) {
			free(temp->ospatar.numeOspatar);
		}
		free(temp);
	}
	*cap = NULL;
}

void dezalocareTabelaDeOspatari(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		dezalocareListaOspatari(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}

float calculeazaMedieLista(Nod* cap) {
	float suma = 0;
	int nrElemente = 0;
	while (cap) {
		suma += cap->ospatar.salariu;
		nrElemente++;
		cap = cap->next;
	}
	return (nrElemente > 0 ? (suma / nrElemente) : 0);
}

float* calculeazaSalariiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* salarii = NULL;
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			(*nrClustere)++;
		}
	}
	salarii = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			salarii[contor] = calculeazaMedieLista(ht.tabela[i]);
			contor++;
		}
	}
	return salarii;
}

Ospatar getOspatarDinLista(Nod* cap, const char* nume) {
	Ospatar o;
	o.id = -1;
	while (cap && strcmp(cap->ospatar.numeOspatar, nume) != 0) {
		cap = cap->next;
	}
	if (cap) {
		o = cap->ospatar;
		o.restaurant = (char*)malloc(strlen(cap->ospatar.restaurant) + 1);
		strcpy(o.restaurant, cap->ospatar.restaurant);
		o.numeOspatar = (char*)malloc(strlen(cap->ospatar.numeOspatar) + 1);
		strcpy(o.numeOspatar, cap->ospatar.numeOspatar);
	}
	return o;
}

Ospatar getOspatarDupaNume(HashTable ht, const char* numeCautat) {
	Ospatar o;
	o.id = -1;
	int poz = calculeazaHash(numeCautat, ht.dim);
	if (poz >= 0 && poz < ht.dim) {
		return getOspatarDinLista(ht.tabela[poz], numeCautat);
	}
	return o;
}

int main() {
	HashTable ht = citireOspatariDinFisier("ospatari.txt", 7);
	afisareTabelaDeOspatari(ht);

	int nrClustere = 0;
	float* salarii = calculeazaSalariiMediiPerClustere(ht, &nrClustere);

	printf("Salariile medii per clustere sunt:\n");
	for (int i = 0; i < nrClustere; i++) {
		printf("%.2f  ", salarii[i]);
	}
	printf("\n");

	Ospatar o = getOspatarDupaNume(ht, "Ion Popescu");

	if (o.id != -1) {
		printf("Ospatarul cautat:\n");
		afisareOspatar(o);
		if (o.restaurant != NULL) {
			free(o.restaurant);
		}
		if (o.numeOspatar != NULL) {
			free(o.numeOspatar);
		}
	}
	else {
		printf("Ospatarul nu a fost gasit.\n");
	}

	dezalocareTabelaDeOspatari(&ht);
	return 0;
}