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
	
	file = fopen(FILE_BLACK, "r");
	if ( file == NULL)
		{
			perror("file blacklist");
			exit(EXIT_FAILURE);
		}
	char *p_search = NULL; 
	char *ligne ;
	char *buffer;
	
	if ( (buffer = (char *)calloc((MAX +1) , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(buffer);
			exit(EXIT_FAILURE);
		}
	 
	 if ( (p_search = (char *)calloc((MAX +1) , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(p_search);
			exit(EXIT_FAILURE);
		}
	if ( (ligne = (char *)calloc(MAX , sizeof(char))) == NULL)
		{
			perror("calloc");
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
		
	
	free(p_search);
	fclose(file);
	return 0;
}

   
int main () 
{
	squidLog *msg  ;
	squidLog ipcMsg_b;
	
	FILE *file = NULL;
	
	if ( (msg = (squidLog*)calloc(1, sizeof(squidLog) ) ) == NULL )
		{
			perror("ERROR_BLACK_CALLOC");
			exit(EXIT_FAILURE);
		} 
	
	doOpenIPC_b(0);
	
	do
		{
			ReadIPC(&msg, ipcMsg_b, msg_id_b); //  Les données dans l'IPC BLACK sont copiées dans le message msg
			sleep(1);
			
			search_forbidden_site(file, msg);
				 
		}  while (msg->mtype != 6);
	 	
	free(msg);
	
	return 0;
	
}
