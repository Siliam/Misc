/*
	- System Programming Assignment
		Create a pipe to stream data between two processes
	
	See lire_fifo.c's description
*/

#include <stdio.h>
#include <stdlib.h>	
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

char mot_fr[100], mot_en[100];
char* traduire(char* mot)
{
	FILE* dict = fopen("dict.txt", "r");
	do {
		fscanf(dict, "%s %s", mot_fr, mot_en);
		if(strcmp(mot, mot_fr) == 0)
			return mot_en;
	} while(!feof(dict));
	return "untrouvable";
}

int main(int argc, char* argv[])
{
	char buffer[129];
	FILE* fichier;
	int fd;
	

	while(1)
	{
		switch(fork())
		{
			case 0:
				fd = open("fictub", O_RDONLY);
				fichier = fdopen(fd, "r");
				fgets(buffer, 128, fichier);
				if(strcmp(buffer, "quitter") == 0)
				{
					close(fd);
					return;
				}	
				printf("%s\n", traduire(buffer));
				close(fd);
				break;
			default:
			{
				wait();
				return;
			}	
		}
	}

	fclose(fichier);
	return 0;
}