#include "orchestrator.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
 
ipc_code Read_IPC()	{
	mbuf	ipcMsg_b;
	ssize_t len = 0;
	key_b = ftok(IPC_NAME_BLACK,id_black);
	int msg_id_b = msgget( key_b, 0 ) ;
	(void) memset( &ipcMsg_b, 0, sizeof(mbuf));	
	printf("BLACK en attente \n");
	if ( (len = msgrcv(msg_id_b,(void *) & ipcMsg_b, 256, 0, 0)) == -1 )	{
		perror("msgcrv failed with error \n");
	    exit(EXIT_FAILURE);
    }
    printf("Reception from orchestrator du message : %s \n",ipcMsg_b.mtext);
    return IPC_SUCCESS ; 
}
   
int main () {
  char **msg = NULL ; 
  int inprocess = 1;
  while (inprocess) Read_IPC();
 return 0;
}

