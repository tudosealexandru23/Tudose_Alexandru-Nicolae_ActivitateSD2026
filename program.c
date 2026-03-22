#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraProgramator {
	int id;
	int aniExperienta;
	float salariu;
	char* nume;
	char* companie;
	unsigned char nivel;
};
typedef struct StructuraProgramator Programator;

typedef struct Nod Nod;
struct Nod {
	Nod* st;
	Nod* dr;
	Programator info;
};

Programator citireProgramatorDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Programator p;

	aux = strtok(buffer, sep);
	p.id = atoi(aux);
	p.aniExperienta = atoi(strtok(NULL, sep));
	p.salariu = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy(p.nume, aux);

	aux = strtok(NULL, sep);
	p.companie = malloc(strlen(aux) + 1);
	strcpy(p.companie, aux);

	p.nivel = *strtok(NULL, sep);
	return p;
}

void afisareProgramator(Programator p) {
	printf("Id: %d\n", p.id);
	printf("Ani experienta: %d\n", p.aniExperienta);
	printf("Salariu: %.2f\n", p.salariu);
	printf("Nume: %s\n", p.nume);
	printf("Companie: %s\n", p.companie);
	printf("Nivel: %c\n\n", p.nivel);
}

void adaugaProgramatorInArbore(Nod** radacina, Programator pNou) {
	if ((*radacina) != NULL) {
		if (pNou.id < (*radacina)->info.id) {
			adaugaProgramatorInArbore(&((*radacina)->st), pNou);
		}
		else if (pNou.id > (*radacina)->info.id) {
			adaugaProgramatorInArbore(&((*radacina)->dr), pNou);
		}
		else {
			free((*radacina)->info.nume);
			free((*radacina)->info.companie);

			(*radacina)->info.aniExperienta = pNou.aniExperienta;
			(*radacina)->info.salariu = pNou.salariu;
			(*radacina)->info.nivel = pNou.nivel;

			(*radacina)->info.nume = malloc(strlen(pNou.nume) + 1);
			strcpy((*radacina)->info.nume, pNou.nume);

			(*radacina)->info.companie = malloc(strlen(pNou.companie) + 1);
			strcpy((*radacina)->info.companie, pNou.companie);
		}
	}
	else {
		(*radacina) = malloc(sizeof(Nod));
		(*radacina)->info = pNou;

		(*radacina)->info.nume = malloc(strlen(pNou.nume) + 1);
		strcpy((*radacina)->info.nume, pNou.nume);

		(*radacina)->info.companie = malloc(strlen(pNou.companie) + 1);
		strcpy((*radacina)->info.companie, pNou.companie);

		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;

	while (!feof(file)) {
		Programator p = citireProgramatorDinFisier(file);
		adaugaProgramatorInArbore(&radacina, p);
		free(p.nume);
		free(p.companie);
	}

	fclose(file);
	return radacina;
}

void afisareProgramatoriInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareProgramatoriInOrdine(radacina->st);
		afisareProgramator(radacina->info);
		afisareProgramatoriInOrdine(radacina->dr);
	}
}

void dezalocareArbore(Nod** radacina) {
	if ((*radacina) != NULL) {
		dezalocareArbore(&((*radacina)->st));
		dezalocareArbore(&((*radacina)->dr));

		free((*radacina)->info.nume);
		free((*radacina)->info.companie);

		free(*radacina);
		*radacina = NULL;
	}
}

Programator getProgramatorByID(Nod* radacina, int id) {
	Programator p;
	p.id = -1;
	if (radacina == NULL) return p;

	if (id > radacina->info.id) {
		return getProgramatorByID(radacina->dr, id);
	}
	else if (id < radacina->info.id) {
		return getProgramatorByID(radacina->st, id);
	}
	else {
		p = radacina->info;

		p.nume = malloc(strlen(radacina->info.nume) + 1);
		strcpy(p.nume, radacina->info.nume);

		p.companie = malloc(strlen(radacina->info.companie) + 1);
		strcpy(p.companie, radacina->info.companie);

		return p;
	}
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina != NULL) {
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	}
	return 0;
}

int main() {
	Nod* arbore = citireArboreDinFisier("programatori.txt");

	afisareProgramatoriInOrdine(arbore);

	Programator p = getProgramatorByID(arbore, 2);
	printf("\n\n---------------\n");
	afisareProgramator(p);

	int i = determinaNumarNoduri(arbore);
	printf("\nNumar programatori: %d\n", i);

	dezalocareArbore(&arbore);
	return 0;
}