#include "orchestrator.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main (int argc, char ** argv) {
	
  int msg_id_b;		/* identificateur de la file de messages */
  mbuf	ipcMsg_b;	 /* message courant */
  
  key_b = ftok(IPC_NAME,id_black);
 
  msg_id_b = msgget( key_b, 0777 ) ;  // pour accéder à la file crée par orchestrator
  msgrcv(msg_id_b, &ipcMsg_b, MAX_SIZE, 0, 0);
   ipcMsg_b.mtype = 1;
	printf("Reception from orchestrator du message %c",ipcMsg_b.mtext[255]);

}

