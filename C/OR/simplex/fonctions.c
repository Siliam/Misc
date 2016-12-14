#include "fonctions.h"

void Simplexe()
{
	int n;		// nombre de variables structurelles
	int m;  	// nombre de contraintes <=
	int a;  	// nombre de contraintes >=, aussi le nombre des variables artificielles
	int e;		// e = m + a, aussi le nombre nombre de variables d'écart ou de contraintes
	int nt; 	// nombre total des variables (structurelles + d'ecart + artificelles)
	int i, j;	// parcours des tableaux
	int p, s;	// vecteur entrant et sortant

	float** A;  // La matrice du système linéaire
	float*  B;  // Le vecteur du second membre
	float*  C;  // Le vecteur des couts
	float*  X;  // Le vecteur de la solution
	int*  	J;  // Le vecteur de la base

	n = Lire("   Entrez le nombre des variables structurelles [n] : ", 2, MAX);
	m = Lire("   Entrez le nombre des contraintes <= : ", 0, MAX);
	a = Lire("   Entrez le nombre des contraintes >= : ", 0, MAX);
	e = m + a;
	nt = n + e + a;

	// Allocation de mémoire
	B = (float*)  malloc(e  * sizeof(float));
	C = (float*)  malloc(nt * sizeof(float));
	X = (float*)  malloc(n  * sizeof(float));
	J = (int*)    malloc(e  * sizeof(int));
	A = (float**) malloc((e + 1) * sizeof(float*));
	for(i = 0; i < e + 1; i++)
	{
		A[i] = (float*) malloc((nt + 1) * sizeof(float));
		for(j = 0; j < nt + 1; j++)
			A[i][j] = 0.0;
	}


	// Lecture des coefficients de A, B et C
	printf("\n   Veuillez saisir les coefficients de A%s: \n", (a) ? " (commencez par les contraintes >=) " : " ");
	for(i = 0; i < e; i++)
	{
		printf("    La ligne A[%d] : ", i+1);
		for(j = 0; j < n; j++)
			scanf("%f", &A[i][a + e + j]);
	}

	printf("\n   Veuillez saisir les %d contraintes%s: \n    B = ", e, (a) ? " (commencez par les contraintes >=) " : " ");
	for(i = 0; i < e; i++)
		scanf("%f", &B[i]);

	printf("\n   Veuillez saisir les %d coefficients de C (les couts) : \n    C = ", n);
	for(i = 0; i < n; i++)
		scanf("%f", &C[a + e + i]);


	// Initialisation du premier tableau du Simplexe
	for(i = 0; i < a; i++)
		A[i][i] = 1;
	for(i = 0; i < e; i++)
		A[i][a + i] = (i >= a) ? 1 : -1;
	for(i = 0; i < a + e; i++)
		C[i] = (i < a) ? (-M) : 0;
	for(i = 0; i < n; i++)
		X[i] = 0;
	for(i = 0; i < e; i++)
		J[i] = (i < a) ? i : a + i;
	for(i = 0; i < e; i++)
		A[i][nt] = B[i];
	A[e][nt] = 0;
	for(i = 0; i < nt; i++)
		A[e][i] = C[i] - calculerZ(C, A, J, i, e);
	A[e][nt] = -calculerZ(C, A, J, nt, e);

	// La boucle principale
	Titre();
	for(i = 0; i < 20; i++)		// Pour empêcher les boucles infinies
	{
		// Pour l'affichage, il faut que le nombre des variables de dépasse pas 7
		if(nt < 8)
			AfficherTableau(A, B, C, J, n, m, a);

		// Si la solution optimale est atteinte
		if(optimal(A[e], nt))
		{
			printf("    >>  Z = %g est maximale pour :\n", -A[e][nt]);

			for(i = 0; i < e; i++)
				if(J[i] >= a + e)	// S'il s'agit d'une variable structurelle
					X[ J[i] - (a + e) ] = A[i][nt];

			for(i = 0; i < n; i++)
				printf("         x%d = %6.4f\n", i + 1, X[i]);
			printf("\n\n\n");
			break;
		}

		// Si la solution diverge
		else if(infini(A, nt, e))
		{
			printf("   >>> Z est infini (polyedre non borne).\n\n\n");
			break;
		}

		// Sinon, continuer à la prochaine iteration
		p = entrant(A[e], nt, J, e);
		s = sortant(A, p, nt, e);

		if(nt < 8)
		{
			printf("    >>  Le vecteur entrant en base est %s\n", nomVecteur(p, a, e));
			printf("    >>  Le vecteur sortant de la base est %s\n\n", nomVecteur(J[s], a, e));
		}

		ProchaineIteration(A, J, nt, e, p, s);
	}
}

void AfficherTableau(float** A, float* B, float* C, int* J, int n, int m, int a)
{
	int e = m + a;
	int nt = n + e + a;
	int i, j;

	// Des espaces pour centrer le tableau
	char spaces[20] = "";
	for(i = 0; i < (80 - ((nt)*8 + 23))/2; i++)
		strcat(spaces, " ");

	printf(spaces);
	printf("+------+-------+------+");
	for(i = 0; i < nt; i++)
		printf("-------+");
	printf("\n");

	printf(spaces);
	printf("| Base |   C   |  XJ  |");
	for(i = 0; i < nt; i++)
		printf("  %s   |", nomVecteur(i, a, e));
	printf("\n");

	printf(spaces);
	printf("+------+-------+------+");
	for(i = 0; i < nt; i++)
		printf("-------+");
	printf("\n");

	for(i = 0; i < e; i++)
	{
		printf(spaces);
		printf("|  %s  |", nomVecteur(J[i], a, e));
		printf(" %5.1f |", C[ J[i] ]);
		printf(" %4.*f |", (A[i][nt] < 99 && A[i][nt] > -99), A[i][nt]);
		for(j = 0; j < nt; j++)
			printf(" %5.1f |", A[i][j]);
		printf("\n");
	}

	printf(spaces);
	printf("+---------------------+");
	for(i = 0; i < nt; i++)
		printf("-------+");
	printf("\n");

	printf(spaces);
	printf("|  Cj - Zj = ");
	printf(" %7.1f |", A[e][nt]);
	for(j = 0; j < nt; j++)
		printf(" %5.*f |", (A[e][j] > 99 || A[e][j] < 99) ? 0 : 1, A[e][j]);
	printf("\n");

	printf(spaces);
	printf("+---------------------+");
	for(i = 0; i < nt; i++)
		printf("-------+");
	printf("\n\n");
}

int optimal(float* CZ, int nt)
{
	// Z est optimal <=> tous les Cj - Zj sont négatifs
	int i;
	for(i = 0; i < nt; i++)
		if(CZ[i] > 0)
			return 0;
	return 1;
}

int infini(float** A, int nt, int e)
{
	// Tester la divergence
	int i, j, f;
	for(j = 0; j < nt; j++)		// Parcourir les Cj - Zj
	{
		if(A[e][j] > 0)				// S'il existe un Cj - Zj > 0 tel que
		{
			f = 1;
			for(i = 0; i < e; i++)
				if(A[i][j] > 0)		// il existe un aij > 0
					f = 0;		// L'algorithme diverge

			if(f)
				return 1;
		}
	}
	return 0;
}

int existe(int x, int* V, int n)
{
	// teste si une valeur existe dans un vecteur
	int i;
	for(i = 0; i < n; i++)
		if(V[i] == x)
			return 1;
	return 0;
}

int entrant(float* CZ, int nt, int* J, int e)
{
	int i, k = 0;
	for(i = 1; i < nt; i++)
		if(CZ[i] > CZ[k] && !existe(i, J, e))
			k = i;
	return k;
}

int sortant(float** A, int p, int nt, int e)
{
	int i, r;
	for(r = 0; A[r][p] <= 0; r++);		// Eviter de commencer par un A[i][p] nul

	for(i = r + 1; i < e; i++)
		if(A[i][p] != 0 && A[i][p] > 0 && (A[i][nt]/A[i][p] < A[r][nt]/A[r][p]))
			r = i;
	return r;
}

float calculerZ(float* C, float** A, int* J, int v, int e)
{
	int i;
	float res = 0;
	for(i = 0; i < e; i++)
		res += C[ J[i] ] * A[i][v];
	return res;
}

void ProchaineIteration(float** A, int* J, int nt, int e, int p, int s)
{
	int i, j;
	float pivot = A[s][p];

	// Mettre à le tableau avec la méthode du rectange
	for(j = 0; j <= nt; j++)
		for(i = 0; i <= e; i++)
			if(j != p && i != s)
			{
				A[i][j] = A[i][j] - A[i][p]*A[s][j]/A[s][p];
			}

	// Mettre à jour la ligne et la colonne du pivot
	for(i = 0; i <= e; i++)
		if(i != s)
			A[i][p] = 0;
	for(i = 0; i <= nt; i++)
		A[s][i] = A[s][i] / pivot;
	A[s][p] = 1.0;

	// Echanger des deux vecteurs
	J[s] = p;
}

char* nomVecteur(int i, int a, int e)
{
	static char nom[5];
	if(i < a)
		sprintf(nom, "v%d", i + 1);
	else if(i < a + e)
		sprintf(nom, "t%d", i - a + 1);
	else
		sprintf(nom, "x%d", i - (a + e) + 1);
	return nom;
}

void Titre()
{
	//  Réinitialisation de l'écran et affichage du titre
	system("CLS");
	printf("  +==========================================================================+\n");
    printf("  |                                                                          |\n");
    printf("  |      Resolution d'un programme lineaire avec la methode du Simplexe      |\n");
    printf("  |                                                                          |\n");
	printf("  +==========================================================================+\n");
	printf("\n\n");
}

int Lire(char* texte, int min, int max)
{
	//  Lecture d'un entier entre min et max
	int v;
	do
	{
		printf(texte);
		scanf("%d", &v);
		fflush(stdin);
	} while(v < min || v > max);
	return v;
}
