#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//folosesc fisierul EX3_ANGAJATI.txt


struct StructuraAngajat {
	int id;
	int Varsta;
	float Salariu;
	char* Nume;
	char* Prenume;
	unsigned char InitialaTatalui;
};
typedef struct StructuraAngajat Angajat;

void afisareAngajat(Angajat angajat) {
	printf("ID:%d\n", angajat.id);
	printf("Varsta:%d\n", angajat.Varsta);
	printf("Salariu:%.2f\n", angajat.Salariu);
	printf("Nume:%s\n", angajat.Nume);
	printf("Prenume:%s\n", angajat.Prenume);
	printf("InitialaTatalui:%c\n\n", angajat.InitialaTatalui);

}

void afisareVectorAngajati(Angajat* EX3_ANGAJATI, int nrAngajati) {
	for (int i = 0; i < nrAngajati; i++)
	{
		afisareAngajat(EX3_ANGAJATI[i]);
	}
}

void adaugaAngajatInVector(Angajat** EX3_ANGAJATI, int* nrAngajati, Angajat angajatNou) {
	Angajat* aux = (Angajat*)malloc(sizeof(Angajat) * ((*nrAngajati) + 1));
	for (int i = 0; i < *nrAngajati; i++)
	{
		aux[i] = (*EX3_ANGAJATI)[i];
	}
	aux[(*nrAngajati)] = angajatNou;
	free(*EX3_ANGAJATI);
	(*EX3_ANGAJATI) = aux;
	(*nrAngajati)++;
}

Angajat citireAngajatFisier(FILE* file) {
	Angajat m;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, file);
	m.id = atoi(strtok(buffer, sep));
	m.Varsta = atoi(strtok(NULL, sep));
	m.Salariu = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	m.Nume = (char*)malloc(strlen(aux) + 1);
	strcpy(m.Nume, aux);
	aux = strtok(NULL, sep);
	m.Prenume = (char*)malloc(strlen(aux) + 1);
	strcpy(m.Prenume, aux);
	m.InitialaTatalui = strtok(NULL, sep)[0];
	return m;


}

Angajat* citireVectorAngajatiFisier(const char* numeFisier, int* nrAngajatiCititi) {
	FILE* f = fopen(numeFisier, "r");
	Angajat* EX3_ANGAJATI = NULL;
	(*nrAngajatiCititi) = 0;
	while (!feof(f))
	{
		adaugaAngajatInVector(&EX3_ANGAJATI, nrAngajatiCititi, citireAngajatFisier(f));
	}
	fclose(f);
	return EX3_ANGAJATI;
}

void dezalocareVectorAngajati(Angajat** vector, int* nrAngajati) {
	for (int i = 0; i < *nrAngajati; i++)
	{
		if (((*vector)[i].Nume) != NULL)
		{
			free((*vector)[i].Nume);
		}

		if (((*vector)[i].Prenume) != NULL)
		{
			free((*vector)[i].Prenume);
		}
		free(*vector);
		(*vector) = NULL;
		(*nrAngajati) = 0;
	}
}

int main() {
	int nrAngajati = 0;
	Angajat* EX3_ANGAJATI = citireVectorAngajatiFisier("EX3_ANGAJATI.txt", &nrAngajati);
	afisareVectorAngajati(EX3_ANGAJATI, nrAngajati);
	dezalocareVectorAngajati(&EX3_ANGAJATI, &nrAngajati);
	return 0;
}