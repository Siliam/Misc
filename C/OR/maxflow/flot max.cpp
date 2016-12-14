#include <iostream>
#include <vector>
#include <iomanip>
#include <stack>

using namespace std;

#define MAX 100

#define NON 0
#define VISITE 1
#define MARQUE 1

int FLUX;//le flot maximum du graphe
int N = 100;//la taille a donner
typedef struct sommet
{
	int num, Marque, Visite;
} Sommet;

typedef struct arret
{
	int Capacite;
	int CapaciteRes;
	int Flot;
} Arret;

Arret Graph[MAX][MAX];	//tableau des arrets du graphe

Sommet Noeud[MAX];		//tableau des sommets du graphe
vector<int> Chemin;		//Le chemin d'augmentation

void AfficherGraph()
{
	cout << "CAPACITES RESIDUELLES" << endl;
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			cout << Graph[i][j].CapaciteRes << " ";
		}
		cout << endl;
	}

	cout << "FLOT" << endl;
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			cout << Graph[i][j].Flot << " ";
		}
		cout << endl;
	}
}

void Initialiser()
{
	int i, j;
	cout << "Donner la taille : ";
	cin >> N;

	for(i = 0; i < N; i++)
	{
		Graph[i][i].Capacite = Graph[i][i].CapaciteRes = 0;
		for(j = 0; j < N; j++)
		{
			if(i!=j)
			{
				cout<< "Donner la capacite de l'arc(" << i << "," << j << ") : ";
				cin >> Graph[i][j].CapaciteRes;
			}
		}
	}

	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			Graph[i][j].Capacite = Graph[j][i].Capacite = Graph[i][j].CapaciteRes > 999;
			Graph[i][j].Flot = Graph[j][i].CapaciteRes;
		}
	}
}

void Initialiser2()
{ //exemple vu en cours avec Mr Samadi,pour le tester modifier dans la fonction principale initialiser() par initialiser2()

	for(int i=0;i<N;i++)
		for(int j=0;j<i;j++){
			Graph[i][j].Capacite=Graph[j][i].Capacite;
			Graph[i][j].CapaciteRes=0;
		}
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++){
			Graph[j][i].Flot=Graph[j][i].Capacite-Graph[j][i].CapaciteRes;
		}
	Graph[5][4].CapaciteRes=1;
	for(int i=0;i<N;i++)
		Noeud[i].num=i;
}

void Marquer(int n)
{
	Noeud[n].Marque = 1;
}

void deMarquer(int n)
{
	Noeud[n].Marque = 0;
}

void Visiter(int n)
{
	Noeud[n].Visite = 1;
}

void deVisiter(int n)
{
	Noeud[n].Visite = 0;
}

int ChercherChemin()
{
	int i,j,trouve;
	Sommet s;

	//on reinitialise les parametres des noeuds
	for(i = 0; i < N; i++)
	{
		deMarquer(i);
		deVisiter(i);
	}

	while(!Chemin.empty()) Chemin.pop_back(); //on vide le tableau chemin

	Marquer(0);
	Visiter(0);
	Chemin.push_back(0);//On marque le sommet de depart

	stack<Sommet> pile;
	pile.push(Noeud[0]);

	while(!pile.empty())
	{
		if(pile.top().num == N-1)
			return 1; //ok chemin trouve,on est arrivé au sommet de destination

		s = pile.top();

		for(trouve = j = 0; j < N; j++)
			if((Graph[s.num][j].CapaciteRes > 0) && (Noeud[j].Marque == 0))
				trouve++;

		if(trouve == 0)
		{
			deVisiter(s.num);Chemin.pop_back();
			pile.pop();//on depile la tete et on la remplace par les voisins non visites
		}

		if(!pile.empty())	s=pile.top();
		for(j=0;j<N;j++)
		{
			if((Graph[s.num][j].CapaciteRes>0)&&(Noeud[j].Marque==0)&&(Noeud[j].Visite==0))
			{
				Marquer(j);Visiter(j);Chemin.push_back(j);
				pile.push(Noeud[j]);
				break;
			}
		}
	}
	return 0; //aucun chemin n'a été trouvé
}

void AfficherChemin()
{
	int p;
	cout << "Le chemin est : ";
	for(p = 0; p < Chemin.size(); p++)
	{
		cout << Chemin.at(p) << "->";
	}
}

int Capacite()
{
	int min = 1000000000;
	int C[MAX];
	int j, p;

	for(p = 0; p < Chemin.size()-1; p++)
	{
		C[p] = Graph[Chemin.at(p)][Chemin.at(p+1)].CapaciteRes; //extraire les capacites a partir de la matrice des arcs
	}

	for(j = 0; j < Chemin.size()-1 ; j++)//trouver le minimum des capacites stockes dans le tableau C
	{
		min = (C[j] < min) ? C[j] : min;
	}

	return min;
}

void Augmenter()
{
	int c = Capacite();
	FLUX += c;

	for(int i = 0; i < Chemin.size()-1 ; i++)
	{
		Graph[Chemin.at(i)][Chemin.at(i+1)].CapaciteRes	-=	c;
		Graph[Chemin.at(i)][Chemin.at(i+1)].Flot		+=	c;
		Graph[Chemin.at(i+1)][Chemin.at(i)].CapaciteRes	+=	c;//le sens contraire
		Graph[Chemin.at(i+1)][Chemin.at(i)].Flot		-=	c;
	}
}

int main()
{
	int i, flot = 0;

	for(i = 0; i < N; i++)
	{
		Noeud[i].num = i;
	}

	Initialiser(); //saisir le graphe
	AfficherGraph();
	cout << endl;

	while( ChercherChemin() )
	{
		//quand chemin existe
		AfficherChemin();
		flot += Capacite();
		cout << endl << "La capacite: "<< Capacite() << endl;
		Augmenter();
		AfficherGraph(); //affiche la capacite residuelle et le flot
		cout << endl;
	}

	cout << endl << "Le flot maximal est : " << flot << endl;
}
