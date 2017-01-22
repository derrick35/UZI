#include "orchestrator.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char ** argv) {
	
  mbuf ipcMsg_r;	 /* message courant */
  int 	msg_id_r;	 /* identificateur de la file de messages */
 
 key_r = ftok(IPC_NAME,id_read);
  
  msg_id_r = msgget( key_r, 0777 ) ;  // pour accéder à la file crée par orchestrator
   ipcMsg_r.mtype = 1;
   strncpy(ipcMsg_r.mtext,"message from READ",255);
  int i;
  for ( i = 0 ; i < 6 ; i++) {
  msgsnd(msg_id_r, &ipcMsg_r, MAX_SIZE,0);  // on envoit un message
  printf("%s",ipcMsg_r.mtext);
  sleep(3);
  	}
	return 0;
}






