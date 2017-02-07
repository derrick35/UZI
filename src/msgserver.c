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

/*
int search_forbidden_site(int file, squidLog *msg)
{
	char *ligne ;
	char *buffer;
	char *p_search = NULL; 
	FILE *projection;
	
	struct stat state_file;
	long length_file;
	
	file = open(FILE_BLACK, O_RDONLY);
	if (file < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	if ( fstat(file, &state_file) != 0)
	{
		perror("stat");
		exit(EXIT_FAILURE);
	}
	length_file = state_file.st_size;
	
	projection = (FILE *) mmap(NULL, length_file, PROT_READ | PROT_WRITE, MAP_PRIVATE, file, 0);
	
	if (projection ==  MAP_FAILED)
	{
		error(0, errno, "mmap");
		perror("mmap");
		exit(EXIT_FAILURE);
	} 
	
	if ( (ligne = (char *)calloc( 1024  , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(ligne);
			exit(EXIT_FAILURE);
		}
	if ( (buffer = (char *)calloc( MAX_SIZE +1 , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(buffer);
			exit(EXIT_FAILURE);
		}
		
	 if ( (p_search = (char *)calloc(MAX_SIZE +1  , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(p_search);
			exit(EXIT_FAILURE);
		} 
		printf("test \n");
	
	if (projection != NULL)
	{
	while( fgets(ligne, MAX_SIZE , projection) != NULL )
		{			printf("test1");
			if ( ( p_search = strstr(ligne, msg->urlDest) ) != NULL)
				{
					printf("on rentre dans p_search \n");
					snprintf(buffer, MAX_SIZE +1 ,"Forbidden url : %s \n", msg->urlDest);
					fprintf(stdout,"%s \n", buffer);
				}
		}
	}
	
	
		free(p_search);
		//free(ligne);
		free(buffer);
		munmap((FILE *) projection, length_file);
		return 0;
	
	

}
   
int main () 
{
	squidLog *msg  ;
	if ( (msg = (squidLog*)calloc(1, sizeof(squidLog) ) ) == NULL )
		{
			perror("ERROR_BLACK_CALLOC");
			exit(EXIT_FAILURE);
		} 
	squidLog ipcMsg_b;
	int file = 0 ;
	doOpenIPC_b(0);
	do
		{
			ReadIPC(&msg, ipcMsg_b, msg_id_b); //  Les données dans l'IPC BLACK sont copiées dans le message msg
			sleep(1);
			
			search_forbidden_site(file, msg);
				 
		}  while (msg->mtype != 6);
	 
	free(msg);
		
	return 0;
	
}		*/


int search_forbidden_site(FILE *file, squidLog *msg)
{
	char *p_search = NULL; 	
	char *ligne ;	
	char *buffer;
	
	
	file = fopen(FILE_BLACK, "r");
	if (file == NULL)
	{
		perror("open");
		exit(EXIT_FAILURE);
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
							snprintf(buffer, MAX_SIZE +1 ,"Forbidden url : %s \n", msg->urlDest);
							fprintf(stdout,"%s \n", buffer);
						}
				}
		}
	free(p_search);
	free(buffer);
	fclose(file);
	return 0; 
}

int main () 
{
	
	squidLog *msg  ;
	
	if ( (msg = (squidLog*)calloc(1, sizeof(squidLog) ) ) == NULL )
		{
			perror("ERROR_BLACK_CALLOC");
			exit(EXIT_FAILURE);
		} 
	squidLog ipcMsg_b;
	
	FILE *file = NULL ;
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
