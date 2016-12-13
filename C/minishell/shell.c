/*
	Create a mini shell (see screenshots)
*/

#include "fonctions.h"

int main(int argc, char* argv[])
{
	char cmd[100];
	char* args[max_args];
	char buffer[100];
	int nb_args, i, j, k;

	do
	{
		// Réserver de la mémoire pour le vecteur d'arguments
		for(i = 0; i < max_args; i++)
			args[i] = (char*) malloc(sizeof(char) * max_arg_taille);

		// Afficher l'entête du shell et demander la saisie de la commande
		afficher_entete();
		fgets(buffer, 100, stdin);

		// Se débarasser des espaces au début
		for(k = 0; buffer[k] == ' '; k++);
		if(k > 0)
			for(j = 0; j <= strlen(buffer) - k; j++)
				buffer[j] = buffer[j + k];
		
		// Extraire la commandes à partir du buffer		
		for(i = 0; buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0'; i++)
		{
			cmd[i] = buffer[i];
		}
		cmd[i] = '\0';

		if(strcmp(cmd, "quitter") != 0)
		{
			// Extraire les arguments à partir du buffer
			for(nb_args = j = i = 0; i <= strlen(buffer); i++)
			{
				if(buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\0')
					args[nb_args][i - j] = buffer[i];
				elset
				{
					// Le dernier arrêt n'était sur un espace
					if(i - j > 0)
					{
						args[nb_args][i - j] = '\0';
						nb_args++;
					}
					j = i + 1;
				}
			}

			// le dernier argument passé doit être nul
			args[nb_args++] = NULL;

			// Si l'utilisateur demande de l'aide
			if(strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0)
			{
				if(nb_args == 2)
					afficher_help("ALL");
				else
					afficher_help(args[1]);
				
				printf(BLEU "\n  Appuyer sur une touche pour continuer .." FIN);
			}

			// S'il demande l'exécution d'un programme
			else
			{
				// ON exécute le programme demandé s'il existe
				printf(BLEU "\n   $ Exécution : " FIN);
				printf("\n\n" FIN);
				if(fork())
				{
					wait();
					printf(BLEU "\n  Appuyer sur une touche pour continuer .." FIN);
				}
				else
				{
					execv(cmd, args);
					printf(JAUNE "  La commande '%s' n'existe pas.\n\n" FIN, cmd);
					printf(JAUNE "  Liste des commandes valides : \n" FIN);
					printf(VERT  "        compiler     concat     lire     salut     quitter     help\n\n" FIN);
					printf(BLEU "\n  Appuyer sur une touche pour continuer .." FIN);
				}
			}
		getchar();
		}
		else
		{
			printf(BLEU "\n   Au revoir ! \n\n" FIN);
		}

		// Libérer de la mémoire pour le vecteur d'arguments
		for(i = 0; i < max_args; i++)
			free(args[i]);

	}
	while(strcmp(cmd, "quitter") != 0);

	return 0;
}
