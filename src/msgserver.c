#include "../include/lib_uzzi.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>

int search_forbidden_site(FILE * file, squidLog *msg)
{
	
	file = fopen("blacklist.txt", "r");
	if ( file == NULL)
		{
			perror("file blacklist");
			exit(EXIT_FAILURE);
		}
	char *p_search = NULL; 
	char *ligne ;
	char *buffer;
	
	if ( (buffer = (char *)malloc((MAX +1) * sizeof(char))) == NULL)
		{
			perror("malloc");
			free(buffer);
			exit(EXIT_FAILURE);
		}
	 
	 if ( (p_search = (char *)malloc((MAX +1) * sizeof(char))) == NULL)
		{
			perror("malloc");
			free(p_search);
			exit(EXIT_FAILURE);
		}
	if ( (ligne = (char *)malloc(1024 * sizeof(char))) == NULL)
		{
			perror("malloc");
			free(ligne);
			exit(EXIT_FAILURE);
		}
	if ( file != NULL)
		{
			while( fgets(ligne, MAX, file) != NULL )
				{
					if ( ( p_search = strstr(ligne, msg->urlDest) ) != NULL)
						{
							snprintf(buffer, MAX +1 ,"Forbidden url : %s \n", msg->urlDest);
							fprintf(stdout,"%s \n", buffer);
						}
				}
		}
		
	fclose(file);
	return 0;
}

   
int main () 
{
	squidLog *msg  ;
	squidLog ipcMsg_b;
	int i ;
	FILE *file = NULL;
	
	if ( (msg = (squidLog*)malloc( sizeof(squidLog) ) ) == NULL )
		{
			error(0, errno, "ERROR_BLACK_MALLOC");
			exit(EXIT_FAILURE);
		} 
	
	doOpenIPC_b(0);
	
	for (i = 0; i < 5 ; i = i+1 ) 
		{
			ReadIPC(&msg, ipcMsg_b, msg_id_b); //  Les données dans l'IPC BLACK sont copiées dans le message msg
			sleep(1);
			
			search_forbidden_site(file, msg);
				 
		}
	 	
	free(msg);
	
	return 0;
	
}
