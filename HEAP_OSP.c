#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraOspatar {
	int id;
	int varsta;
	float salariu;
	char* nivelExperienta;
	char* nume;
	unsigned char cod;
};
typedef struct StructuraOspatar Ospatar;

struct Heap {
	int lungime;
	Ospatar* vector;
	int nrOspatari;
};
typedef struct Heap Heap;

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
	o.nivelExperienta = malloc(strlen(aux) + 1);
	strcpy_s(o.nivelExperienta, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	o.nume = malloc(strlen(aux) + 1);
	strcpy_s(o.nume, strlen(aux) + 1, aux);

	o.cod = *strtok(NULL, sep);
	return o;
}

void afisareOspatar(Ospatar o) {
	printf("Id: %d\n", o.id);
	printf("Varsta: %d\n", o.varsta);
	printf("Salariu: %.2f\n", o.salariu);
	printf("Nivel Experienta: %s\n", o.nivelExperienta);
	printf("Nume: %s\n", o.nume);
	printf("Cod: %c\n\n", o.cod);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrOspatari = 0;
	heap.vector = (Ospatar*)malloc(sizeof(Ospatar) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (pozFiuSt < heap.nrOspatari && heap.vector[pozMax].salariu < heap.vector[pozFiuSt].salariu) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrOspatari && heap.vector[pozMax].salariu < heap.vector[pozFiuDr].salariu) {
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod) {
		Ospatar aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrOspatari - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeOspatariDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrOspatari++] = citireOspatarDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrOspatari - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrOspatari; i++) {
		afisareOspatar(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrOspatari; i < heap.lungime; i++) {
		afisareOspatar(heap.vector[i]);
	}
}

Ospatar extrageOspatar(Heap* heap) {
	if (heap->nrOspatari > 0) {
		Ospatar aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrOspatari - 1];
		heap->vector[heap->nrOspatari - 1] = aux;
		heap->nrOspatari--;
		for (int i = (heap->nrOspatari - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return aux;
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].nivelExperienta);
		free(heap->vector[i].nume);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrOspatari = 0;
}

int main() {
	Heap heap = citireHeapDeOspatariDinFisier("ospatari.txt");
	afisareHeap(heap);
	printf("Ospatari extrasi:\n");
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));
	afisareOspatar(extrageOspatar(&heap));

	printf("Heap-ul ascuns:\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);
	return 0;
}