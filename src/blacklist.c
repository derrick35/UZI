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
#include <sys/mman.h>
#include <fcntl.h>


int search_forbidden_site(FILE *file, squidLog *msg)
{
	char *p_search = NULL; 	
	char *ligne ;	
	char *buffer;
	
	
	file = fopen(FILE_BLACK, "r");
	if (file == NULL)
	{
		if (errno == EINTR)
			{
				do {file = fopen(FILE_BLACK, "r"); }
				while (errno == EINTR);
			}
		else 
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
	}
	
	if ( (buffer = (char *)calloc((MAX_SIZE +1 ) , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(buffer);
			exit(EXIT_FAILURE);
		}
	 
	 if ( (p_search = (char *)calloc((MAX_SIZE +1) , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(p_search);
			exit(EXIT_FAILURE);
		}
	if ( (ligne = (char *)calloc(1024 , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(ligne);
			exit(EXIT_FAILURE);
		}
	if ( file != NULL)
		{
			while( fgets(ligne, MAX_SIZE , file) != NULL )
				{
					if ( ( p_search = strstr(ligne, msg->urlDest) ) != NULL)
						{
							snprintf(buffer, MAX_SIZE +1 ,"Forbidden url : %s from %s with IP adress %s \n", msg->urlDest, msg->user,msg->clientIpAdress);
							fprintf(stdout,"%s \n", buffer);
						}
				}
		}
		
	free(p_search);
	free(ligne);
	free(buffer);
	fclose(file);
	return 0; 
}

int main () 
{
	squidLog ipcMsg_b;
	squidLog *msg  ;
	
	if ( (msg = (squidLog*)calloc(1, sizeof(squidLog) ) ) == NULL )
		{
			perror("ERROR_BLACK_CALLOC");
			exit(EXIT_FAILURE);
		} 
	
	/* Blacklist will have no privilege and belong to uzi group  */	 
	change_ids(nobody, uzi);

	FILE *file = NULL ;
	doOpenIPC_b(0);
	
	do
		{
			if ( ReadIPC(&msg, ipcMsg_b, msg_id_b) == IPC_ERROR_R ) 
				{
					CloseIPC_b();
				}
			
			search_forbidden_site(file, msg);
			
				 
		}  while (msg->mtype != 6);
		
	free(msg);
		
	return 0;
	
}		
