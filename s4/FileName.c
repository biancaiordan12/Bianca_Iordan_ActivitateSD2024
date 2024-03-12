#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

struct Masina {
	int id;
	char* producator;
	int nrUsi;
	float pret;
};

struct Masina* adaugareInVector(struct Masina* masini, struct Masina m, int* nrMasini) {
	struct Masina* aux = (struct Masina*)malloc(sizeof(struct Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < *nrMasini; i++)
	{
		aux[i] = masini[i];
	}
	aux[*nrMasini] = m;
	(*nrMasini)++;
	if (masini != NULL)
		free(masini);
	return aux;
}

struct Masina* citireFisier(const char* numeFisier, int* nrMasini)
{
	FILE* f = fopen(numeFisier, "r");
	char buffer[100];
	char delimitator[] = ",\n";
	struct Masina** masini = NULL;
	(*nrMasini) = 0;
	while (fgets(buffer, 100, f) != NULL) {
		char* token = strtok(buffer, delimitator);
		struct Masina m;
		m.id = atoi(token);
		token = strtok(NULL, delimitator);
		m.producator = malloc(strlen(token) + 1);
		strcpy(m.producator, token);
		token = strtok(NULL, delimitator);
		m.nrUsi = atoi(token);
		token = strtok(NULL, delimitator);
		m.pret = atof(token);
		masini = adaugareInVector(masini, m, nrMasini);
	}
	fclose(f);
	return masini;
}

void citireMatrice(const char* numeFisier, struct Masina** matrice, int nrLinii, int* nrCol)
{
	FILE* f = fopen(numeFisier, "r");
	char buffer[100];
	char delimitator[] = ",\n";
	while (fgets(buffer, 100, f) != NULL) {
		char* token = strtok(buffer, delimitator);
		struct Masina m;
		m.id = atoi(token);
		token = strtok(NULL, delimitator);
		m.producator = malloc(strlen(token) + 1);
		strcpy(m.producator, token);
		token = strtok(NULL, delimitator);
		m.nrUsi = atoi(token);
		token = strtok(NULL, delimitator);
		m.pret = atof(token);
		matrice[m.nrUsi - 2] = adaugareInVector(matrice[m.nrUsi - 2], m, &(nrCol[m.nrUsi - 2]));
	}
	fclose(f);
}

void afisareMatrice(struct Masina** matrice, int nrLinii, int nrCol)
{
	for (int i = 0; i < nrLinii; i++)
		for (int j = 0; j < nrCol; j++)
		{
			printf("\n%d %s %d %.2f \t", matrice[i][j].id, matrice[i][j].producator, matrice[i][j].nrUsi, matrice[i][j].pret);
		}
}

//pretul mediu al masinilor cu nr de usi dat ca parametru
float pretMediu(struct Masina** matrice, int nrLinii, int* nrCol, int nrUsi)
{
	if (nrUsi < 2 || nrUsi>4)
		return 0;
	float pretTotal = 0;
	int index = nrUsi - 2;
	if (nrCol[index] == 0)
		return 0;
	for (int i = 0; i < nrCol[index]; i++)
	{
		pretTotal += matrice[index][i].pret;
	}
	return pretTotal / nrCol[index];
}

struct Masina cautaMasinaID(struct Masina** matrice, int nrLinii, int* nrCol, int id)
{
	for (int i = 0; i < nrLinii; i++)
	{
		for (int j = 0; j < nrCol[i]; j++)
		{
			if (matrice[i][j].id == id)
				return matrice[i][j];
		}
	}
	struct Masina masina;
	masina.id = -1;
	masina.producator = NULL;
	masina.nrUsi = -1;
	masina.pret = -1;
	return masina;
}

void dezalocareMatrice(struct Masina*** matrice, int nrLinii, int** nrCol) //inca o * pt ce vom modifica
{
	for (int i = 0; i < nrLinii; i++)
	{
		for (int j = 0; j < (*nrCol)[i]; j++) {
			free((*matrice)[i][j].producator);
		}
		free((*matrice)[i]);
	}
	free((*nrCol));
	(*nrCol) = NULL;
	free((*matrice));
	(*matrice) = NULL;
}

int main()
{
	struct Masina** matrice;
	int nrLinii = 3;
	int* nrCol;
	nrCol = (int*)malloc(sizeof(int) * nrLinii);
	matrice = (struct Masina**)malloc(sizeof(struct Masina*) * nrLinii);
	for (int i = 0; i < nrLinii; i++)
	{
		nrCol[i] = 0;
		matrice[i] = NULL;
	}
	citireMatrice("masini.txt", matrice, nrLinii, nrCol);
	afisareMatrice(matrice, nrLinii, nrCol);
	printf("\nPretul mediu al masinilor cu 3 usi este: %5.2f\n", pretMediu(matrice, nrLinii, nrCol, 3));
	struct Masina masina_cautata = cautaMasinaID(matrice, nrLinii, nrCol, 6);
	printf("\n%d %s %d %.2f\n", masina_cautata.id, masina_cautata.producator, masina_cautata.nrUsi, masina_cautata.pret);
	dezalocareMatrice(&matrice, nrLinii, &nrCol);

}