#include "orchestrator.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <bsd/string.h>

int main () {
	
  printf("test");
  int msg_id_b;		/* identificateur de la file de messages */
  mbuf	ipcMsg_b;	 /* message courant */
  (void) memset( &ipcMsg_b, 0, sizeof(mbuf));
  
  key_b = ftok(IPC_NAME,id_black);
  msg_id_b = msgget( key_b, 0777 ) ;  // pour accéder à la file crée par orchestrator
  printf("BLACK en attente");
  for (;;) {
  msgrcv(msg_id_b, &ipcMsg_b, 255, 0, 0);}
   	printf("Reception from orchestrator du message : %s \n",ipcMsg_b.mtext);

	return 0;
}
/*
void *read_thread(void *arg) {
	(void) arg ;
	printf("Ouverture IPC read \n");
	CreateIPC_r() ; 
	printf("%d \n",msg_id_r);
	ReadIPC();
	pthread_exit(NULL);

mbuf ipcMsg_r;
	(void) memset( &ipcMsg_r, 0, sizeof(mbuf));
	//for (;;) {
	msgrcv(msg_id_r, (void *) &ipcMsg_r, 255, 0, 0);
	//*msg = (char *) malloc(len);
	//(void) memcpy(*msg, ipcMsg_r.mtext, len);
	printf("Reception sur orchestrator : %s \n",ipcMsg_r.mtext); 
	CloseIPC_r();
	printf("ReadIPC est ferme \n");
*/
