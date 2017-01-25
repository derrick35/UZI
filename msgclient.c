#include "orchestrator.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main () {
	
  mbuf ipcMsg_r;	 /* message courant */
  int 	msg_id_r;	 /* identificateur de la file de messages */
 
 key_r = ftok(IPC_NAME_READ,id_read);
  printf("Envoi de messages vers orchestrator \n");
  msg_id_r = msgget( key_r, 0 ) ;  // pour accéder à la file crée par orchestrator
   ipcMsg_r.mtype = 1;
   strncpy(ipcMsg_r.mtext,"message from READ",255);
  int i;
   for ( i = 0 ; i < 6 ; i++) {
  msgsnd(msg_id_r, &ipcMsg_r, MAX_SIZE, 0);  // on envoit 6 messages
   sleep(3);
  	}
	return 0;
}






