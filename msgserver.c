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
#include <fcntl.h>
 

   
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
	
	doOpenIPC_b(0);
	
	for (i = 0; i < 5 ; i = i+1 ) 
	{
	 ReadIPC(&msg, ipcMsg_b, msg_id_b); //  Les données dans l'IPC BLACK sont copiées dans le message msg
	 
	 printf("Lecture par blacklist : %s / %ld / %s \n",msg->mtext, msg->ad_IP, msg->autre); 
	}
	
	free(msg);
	
	return 0;
	
}

