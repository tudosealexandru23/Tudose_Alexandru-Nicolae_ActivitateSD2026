#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Sablon {
	int id;
	char* masina;
	char* reparatie;
	float cost;
};

struct Sablon initializare(int id, const char* masina, const char* reparatie, float cost) {
	struct Sablon s;
	s.id = id;

	s.masina = (char*)malloc(strlen(masina) + 1);
	if (s.masina != NULL) {
		strcpy_s(s.masina, strlen(masina) + 1, masina);
	}

	s.reparatie = (char*)malloc(strlen(reparatie) + 1);
	if (s.reparatie != NULL) {
		strcpy_s(s.reparatie, strlen(reparatie) + 1, reparatie);
	}

	s.cost = cost;

	return s;
}

void afisare(struct Sablon s) {
	printf("ID: %d\n", s.id);
	printf("Masina: %s\n", s.masina);
	printf("Reparatie: %s\n", s.reparatie);
	printf("Cost: %.2f\n", s.cost);
	printf("\n");
}

void modifica_Atribut(struct Sablon* s, float costNou) {
	s->cost = costNou;
}

void dezalocare(struct Sablon* s) {
	free(s->masina);
	free(s->reparatie);
}

int main() {

	struct Sablon s;

	s = initializare(1, "BMW", "Schimb ulei", 1000);

	afisare(s);

	modifica_Atribut(&s, 350);

	printf("Dupa modificare:\n");
	afisare(s);

	dezalocare(&s);

	return 0;
}