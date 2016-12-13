/*
	- System Programming Assignment : 
		Create a pipe to stream data between two processes
	
	Compile both ths and traduire_fifo.c
	Launch each on a seperate terminal instance
	write into ./lire_fifo and the translated words will go to ./ecrire-fifo output
	Words should be included in dic.txt
*/

#include <stdio.h>
#include <stdlib.h>	
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char* argv[])
{
	mode_t mode = S_IRUSR | S_IWUSR;
	int tub;
	char buf[100];

	mkfifo("fictub", mode);
	
	while(1)
	{
		scanf("%s", buf);
		tub = open("fictub", O_WRONLY);
		write(tub, buf, strlen(buf));
		close(tub);
		if(strcmp(buf, "quitter") == 0)
		{
			break;
		}
	}


	return 0;
}