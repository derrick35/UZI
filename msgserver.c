#include "config.h"
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

int search_forbidden_site(FILE * file, mbuf *msg)
{
	file = fopen("blacklist.txt", "r");
	char *p_search = NULL; 
	char ligne[1024] ;
	
	if ( file != NULL)
		{
			while( fgets(ligne, MAX_SIZE, file) != NULL )
				{
					if ( ( p_search = strstr(ligne, msg->mtext) ) != NULL)
						{
							printf("Forbidden url : %s \n", msg->mtext);
						}
				}
		}
		
	fclose(file);
	return 0;
}

   
int main () 
{
	mbuf *msg  ;
	mbuf ipcMsg_b;
	int i ;
	FILE *file = NULL;
	
	if ( (msg = (mbuf*)malloc( sizeof(mbuf) ) ) == NULL )
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
			printf("Lecture par blacklist : %s / %s / %s \n",msg->mtext, msg->ad_IP, msg->autre); 
	 
		}
	 	
	free(msg);
	
	return 0;
	
}
