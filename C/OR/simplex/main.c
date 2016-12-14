#include "fonctions.h"

int main()
{
	int continuer;
	do
	{
		Titre();
		Simplexe();
		continuer = Lire("    Voulez-vous recommencer ? (1/0) : ", 0, 1);
	} while(continuer);
}
