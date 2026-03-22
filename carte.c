#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
	unsigned char categorie;
};
typedef struct StructuraCarte Carte;

typedef struct Nod Nod;
struct Nod {
	Nod* st;
	Nod* dr;
	Carte info;
};

Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Carte c1;

	aux = strtok(buffer, sep);
	if (aux == NULL) return c1;

	c1.id = atoi(aux);
	c1.nrPagini = atoi(strtok(NULL, sep));
	c1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c1.titlu = malloc(strlen(aux) + 1);
	strcpy_s(c1.titlu, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c1.autor = malloc(strlen(aux) + 1);
	strcpy_s(c1.autor, strlen(aux) + 1, aux);

	c1.categorie = *strtok(NULL, sep);
	return c1;
}

void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n", carte.autor);
	printf("Categorie: %c\n\n", carte.categorie);
}

void adaugaCarteInArbore(Nod** radacina, Carte carteNoua) {
	if ((*radacina) != NULL) {
		if (carteNoua.id < (*radacina)->info.id) {
			adaugaCarteInArbore(&((*radacina)->st), carteNoua);
		}
		else if (carteNoua.id > (*radacina)->info.id) {
			adaugaCarteInArbore(&((*radacina)->dr), carteNoua);
		}
		else {
			free((*radacina)->info.titlu);
			free((*radacina)->info.autor);

			(*radacina)->info.nrPagini = carteNoua.nrPagini;
			(*radacina)->info.pret = carteNoua.pret;
			(*radacina)->info.categorie = carteNoua.categorie;

			(*radacina)->info.titlu = malloc(strlen(carteNoua.titlu) + 1);
			strcpy_s((*radacina)->info.titlu, strlen(carteNoua.titlu) + 1, carteNoua.titlu);

			(*radacina)->info.autor = malloc(strlen(carteNoua.autor) + 1);
			strcpy_s((*radacina)->info.autor, strlen(carteNoua.autor) + 1, carteNoua.autor);
		}
	}
	else {
		(*radacina) = malloc(sizeof(Nod));
		(*radacina)->info = carteNoua;

		(*radacina)->info.titlu = malloc(strlen(carteNoua.titlu) + 1);
		strcpy_s((*radacina)->info.titlu, strlen(carteNoua.titlu) + 1, carteNoua.titlu);

		(*radacina)->info.autor = malloc(strlen(carteNoua.autor) + 1);
		strcpy_s((*radacina)->info.autor, strlen(carteNoua.autor) + 1, carteNoua.autor);

		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

Nod* citireArboreDeCartiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;

	while (!feof(file)) {
		Carte c = citireCarteDinFisier(file);
		adaugaCarteInArbore(&radacina, c);
		free(c.titlu);
		free(c.autor);
	}

	fclose(file);
	return radacina;
}

void afisareCartiInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareCartiInOrdine(radacina->st);
		afisareCarte(radacina->info);
		afisareCartiInOrdine(radacina->dr);
	}
}

void dezalocareArboreDeCarti(Nod** radacina) {
	if ((*radacina) != NULL) {
		dezalocareArboreDeCarti(&((*radacina)->st));
		dezalocareArboreDeCarti(&((*radacina)->dr));

		free((*radacina)->info.titlu);
		free((*radacina)->info.autor);

		free(*radacina);
		*radacina = NULL;
	}
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina != NULL) {
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	}
	return 0;
}

int main() {
	Nod* arbore = citireArboreDeCartiDinFisier("carti.txt");

	afisareCartiInOrdine(arbore);

	int nr = determinaNumarNoduri(arbore);
	printf("\nNumar carti: %d\n", nr);

	dezalocareArboreDeCarti(&arbore);
	return 0;
}