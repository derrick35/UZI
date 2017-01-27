#include "config.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

ipc_code Write_IPC(char *msg)	{
	mbuf *ipcMsg_r = NULL;
	if (msg == NULL) { return IPC_ERROR ;  }
	int 	msg_id_r = 0;			/* identificateur de la file de messages */
	key_r = ftok(IPC_NAME_READ,id_read);
  	msg_id_r = msgget( key_r, 0 ) ;  // pour accéder à la file crée par orchestrator
	if ( (ipcMsg_r =  (mbuf * ) malloc(sizeof(mbuf)) ) == NULL) { return IPC_ERROR ; }
	ipcMsg_r->mtype = 42;
	strncpy(ipcMsg_r->mtext,msg, MAX_SIZE);
	int i;
	for ( i = 0 ; i < 6 ; i++) {
	if ( msgsnd(msg_id_r, ipcMsg_r, strlen(ipcMsg_r->mtext) + 1 , 0) == -1 ) { return IPC_ERROR ; }
	sleep(3);	}
	free(ipcMsg_r);
	return IPC_SUCCESS ; 
}

int main () {
	  
	printf("Envoi de messages vers orchestrator \n");
	char *msg = "message from READ" ;
	int i = 0;
	for (i = 0 ; i < 6 ; i = i + 1 )
		{
			Write_IPC(msg);
  	     }
	return 0;
}






