#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 1000
#define M 99

void Simplexe();
void AfficherTableau(float** A, float* B, float* C, int* J, int n, int m, int a);
int optimal(float* CZ, int nt);
int infini(float** A, int nt, int e);
int existe(int x, int* V, int n);
int entrant(float* CZ, int nt, int* J, int e);
int sortant(float** A, int p, int nt, int e);
float calculerZ(float* C, float** A, int* J, int v, int e);
void ProchaineIteration(float** A, int* J, int nt, int e, int p, int s);
char* nomVecteur(int i, int a, int e);
void Titre();
int Lire(char* texte, int min, int max);
