#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#define NMAX 100

using namespace std ;

class dijkstra
{
private:
	int NbrSommets;
	int source;		// Sommet de départ
	int parcours[NMAX];	// Parcourus ? 0 | 1
	int distance[NMAX];	// D[i]
	int matrice[NMAX][NMAX]; 	// Matrice du graphe
	int predecesseur[NMAX];	// ?
	int set[NMAX];		// Ordre des sommets visités

public:
	int minimum();
	void lecture();
	void initialiser();
	void traitement();
	void affiche_chemin(int i);
	void affiche();
};


#endif // DIJKSTRA_H
