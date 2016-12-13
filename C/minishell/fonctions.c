#include "fonctions.h"

void afficher_entete()
{
	system("clear");
	printf(JAUNE);
	printf("\t\t\t   =======================\n");
	printf("\t\t\t   ==       Shell       ==\n");
	printf("\t\t\t   =======================\n\n" FIN);
	printf("  Entrez une commande (ou help pour afficher l'aide) : \n\n");
	printf(BLEU "  $ " FIN);
}

void afficher_help(char* cmd)
{
	int existe = 0;
	printf("\n");

	if(strcmp(cmd, "lire") == 0 || strcmp(cmd, "ALL") == 0)
	{	
		existe = 1;
		printf("\n");
		printf(JAUNE  "    lire\n");
		printf(VERT "\t Description : " FIN " Afficher le contenu du fichier passé en argument.\n");
		printf(VERT "\t Syntaxe     : " BLEU " $ " FIN "lire fichier\n\n");	
	}
	if(strcmp(cmd, "concat") == 0 || strcmp(cmd, "ALL") == 0)
	{	
		existe = 1;
		printf(JAUNE  "    concat\n");
		printf(VERT "\t Description : " FIN " Concaténer les fichiers passés en arguments \n");
		printf(VERT "\t Syntaxe     : " BLEU " $ " FIN "concat f1 f2 [f3 ..] resultat\n\n");
	}
	if(strcmp(cmd, "compiler") == 0 || strcmp(cmd, "ALL") == 0)
	{	
		existe = 1;
		printf(JAUNE  "    compiler\n");
		printf(VERT  "\t Description :  " FIN "Compiler le programme passé en argument.\n");
		printf(VERT  "\t Syntaxe     : " BLEU " $ " FIN "compile nom_prog_a_compiler\n\n");
	}
	if(strcmp(cmd, "salut") == 0 || strcmp(cmd, "ALL") == 0)
	{	
		existe = 1;
		printf(JAUNE  "    salut\n");
		printf(VERT  "\t Description :  " FIN "Dire \"Salut!\" aux noms passés.\n");
		printf(VERT  "\t Syntaxe     : " BLEU " $ " FIN "Salut nom1 [, nom2 ..]\n\n");
	}

	if(!existe)
	{
		existe = 1;
		printf(ROUGE  "\n    La commande %s n'existe pas, Veuillez réessayer.\n\n" FIN, cmd);
	}
}
