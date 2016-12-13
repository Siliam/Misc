#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ROUGE  "\e[31m"
#define VERT   "\e[32m"
#define JAUNE  "\e[33m"
#define BLEU   "\e[36m"
#define FIN    "\e[m"

#define max_args 20
#define max_arg_taille 50

void afficher_entete();
void afficher_help();
