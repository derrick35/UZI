#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>
 

int compare(FILE * file, mbuf *msg)
{
	char ligne[MAX_SIZE] ;
	char * word;
	char *pointeur ; 
	
	if ( file != NULL)
	{
		while( fgets(ligne, MAX_SIZE, file) != NULL )
			{
				 /*word = strtok_r(ligne, " ", &pointeur);
				printf("%s \n", word);
				word = strtok_r(NULL, " ", &pointeur);
				printf("%s \n", word);
				word = strtok_r(NULL, " ", &pointeur);
				printf("%s \n", word);	*/
					
						//printf("%s \n",word);
						if ( (word = strtok_r(ligne, " ", &pointeur)) == (msg->mtext) )
						{
							printf("%s \n",word);
							if ( (word = strtok_r(NULL, " ", &pointeur))  == (msg->ad_IP) )
								{
									printf("%s",word);
									if ( (word = strtok_r(NULL, " ", &pointeur)) == (msg->autre) )
										{
											printf("%s",word);
											printf(" Forbidden website : %s ", ligne);
										}	
								}
						}
					
			
			
			/*if ( ligne[1] == (*msg->mtext)  || 
					ligne[2] == msg->ad_IP  || 
						ligne[3]== (*msg->autre) )
						{
							printf(" Forbidden website : %s ", ligne);
						}		*/
				//printf("%s \n", ligne[3]);
				//printf("%s \n", msg->mtext );
				//printf("%s \n", ligne[2]);
				//printf("%s \n", ligne[3]);
			} 
		}
	
	
	return 0;
}

   
int main () 
{
	mbuf *msg  ;
	if ( (msg = (mbuf*)malloc( sizeof(mbuf) ) ) == NULL )
	{
		error(0, errno, "ERROR_BLACK_MALLOC");
		exit(EXIT_FAILURE);
	} 
	mbuf ipcMsg_b;
	int i ;
	
	FILE *file = NULL;
	file = fopen("blacklist.txt", "r");
	
	doOpenIPC_b(0);
	
	for (i = 0; i < 5 ; i = i+1 ) 
	{
	 ReadIPC(&msg, ipcMsg_b, msg_id_b); //  Les données dans l'IPC BLACK sont copiées dans le message msg
	 sleep(1);
	 compare(file, msg);
	 
	 printf("Lecture par blacklist : %s / %s / %s \n",msg->mtext, msg->ad_IP, msg->autre); 
	 
	 }
	 
	 fclose(file);
	
	free(msg);
	
	return 0;
	
}

