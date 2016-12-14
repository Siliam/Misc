#include "dijkstra.h"

int dijkstra::minimum()
{
	int min = 999;
	int i, t;

	for(i = 1; i <= NbrSommets; i++)
	{
		if (parcours[i] != 1 && min >= distance[i])
		{
			min = distance[i];
			t = i;
		}
	}

	return t;
}

void dijkstra::lecture()
{
	string reponse;

	cout << "Voulez vous utiliser un fichier pour lire le graphe (oui / non) ? ";
	cin  >> reponse;

	if(reponse == "oui" || reponse == "Oui" || reponse == "O" || reponse == "o")
	{
		cout << "Veuillez saisir le nom du fichier : ";
		cin  >> reponse;
		ifstream fichier(reponse, ifstream::in);

		if(fichier.good())
		{
			fichier >> NbrSommets;
			cout << "Nombre de sommets : " << NbrSommets << endl;

			if(NbrSommets < 1)
			{
				cout << "Erreur : Nombre de sommets non valide." << endl;
				return;
			}

			cout << endl << "Lecture de la matrice des distance (999 si la distance est infini) .." << endl;

			for(int i = 1; i <= NbrSommets; i++)
			{
				for(int j = 1; j <= NbrSommets; j++)
				{
					if(fichier.eof())
					{
						cout << "Erreur : Matrice de graphe incomplete." << endl;
						return;
					}

					fichier >> matrice[i][j];
					cout 	<< matrice[i][j] << "\t";

					if(matrice[i][j] < 0)
					{
						cout <<"Erreur : Les distance doivent être positives." << endl;
						return;
					}
				}
			}

			fichier.close();
		}
		else
			cout << "Erreur. Fichier 'graphe.txt' introuvable" << endl;
	}

	else
	{
		cout << endl << "Donner le nombre de sommets : ";
		cin  >> NbrSommets;

		while(NbrSommets < 1)
		{
			cout << "Nombre de sommets non valide. Veuillez saisir un autre : ";
			cin >> NbrSommets;
		}

		cout << endl << "Entrer la matrice des distance (999 si la distance est infini) :" << endl;

		for(int i = 1; i <= NbrSommets; i++)
		{
			cout<<"Entrer les valeurs de la ligne " << i << " : "<< endl;
			for(int j = 1; j <= NbrSommets; j++)
			{
				if(i == j)
				{
					matrice[i][j] = 0;
					continue;
				}
				cout << "  " << i << " > " << j << " : ";
				cin  >> matrice[i][j];
				while(matrice[i][j] < 0)
				{
					cout << endl << "Les distance doivent être positives. Refaire cette saisie : "<<endl;
					cin >> matrice[i][j];
				}
			}
		}
	}
	cout << endl << "Entrer sommets de depart : ";
	cin  >> source;
	system("CLS");
}


void dijkstra::initialiser()
{
	for(int i = 1; i <= NbrSommets; i++)
	{
		parcours[i] = 0;
		distance[i] = 999;
		predecesseur[i] = 0;
	}

	distance[source] = 0;
}

void dijkstra::traitement()
{
	int count = 0;
	int i, u;

	initialiser();

	while(count < NbrSommets)
	{
		u = minimum();
		set[++count] = u;
		parcours[u]  = 1;

		for(i = 1; i <= NbrSommets; i++)
			if(matrice[u][i] > 0 && parcours[i] != 1) // il existe un chemin entre u et i et i n'est pas visité
			{
				if(distance[i] > (distance[u] + matrice[u][i]))	// Mise à jour des distances
				{
					distance[i]	= distance[u] + matrice[u][i];
					predecesseur[i] = u; 	// Pour retracer le chemin
				}
			}
	}
}

void dijkstra::affiche_chemin(int i)
{
	if(i == source)
		cout << source;

	else if (predecesseur[i] == 0)
		cout<<"Pas de chemin de "<< source << " a " << i;
	else
	{
		affiche_chemin(predecesseur[i]);
		cout << " -> " << i;
	}
}

void dijkstra::affiche()
{
	cout << "Ordre de parcours : " << set[1];
	for(int i = 2; i <= NbrSommets; i++)
		cout << " > " << set[i];
	cout << endl << endl;

	for(int i = 1; i <= NbrSommets; i++)
	{
		cout << " - Le plus court chemin de " << source << " a " << i << " : ";
		affiche_chemin(i);
		cout << endl;
		cout << " - La distance minimale entre " << source << " et " << i << " est : " << distance[i] << endl << endl;
	}
}
