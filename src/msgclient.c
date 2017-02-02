#include "../include/lib_uzzi.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main () 
{
	int msg_id_r = 1;
	printf("Envoi de messages vers orchestrator \n");
	doOpenIPC_r(0);
	mbuf* ipcMsg_r;
	ipcMsg_r = (mbuf*)malloc( sizeof(mbuf) ) ;
	
	mbuf *msg  ;
	msg = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg->mtype = 42;
	strlcpy(msg->mtext, "message0", MAX_SIZE);
	strlcpy(msg->autre, "autrechamp0", 20);
	strlcpy(msg->ad_IP, "168", 20);
	WriteIPC(msg, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	
	sleep(2);
	mbuf *msg1  ;
	msg1 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg1->mtype = 42;
	strlcpy(msg1->mtext, "message1", MAX_SIZE);
	strlcpy(msg1->autre, "autrechamp1", 20);
	strlcpy(msg1->ad_IP, "169", 20);
	WriteIPC(msg1, ipcMsg_r, msg_id_r);

	sleep(2);
	mbuf *msg2  ;
	msg2 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg2->mtype = 42;
	strlcpy(msg2->mtext, "message2", MAX_SIZE);
	strlcpy(msg2->autre, "autrechamp2", 20);
	strlcpy(msg2->ad_IP, "170", 20);
	WriteIPC(msg2, ipcMsg_r, msg_id_r);
	
	sleep(2);
	mbuf *msg3  ;
	msg3 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg3->mtype = 42;
	strlcpy(msg3->mtext, "message3", MAX_SIZE);
	strlcpy(msg3->autre, "autrechamp3", 20);
	strlcpy(msg3->ad_IP, "171", 20);
	WriteIPC(msg3, ipcMsg_r, msg_id_r);
	
	sleep(2);
	mbuf *msg4  ;
	msg4 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg4->mtype = 42;
	strlcpy(msg4->mtext, "message4", MAX_SIZE);
	strlcpy(msg4->autre, " autrechamp4", 20);
	strlcpy(msg4->ad_IP, "172", 20);
	WriteIPC(msg4, ipcMsg_r, msg_id_r);
	
	
	
	//Quand on atteint la fin du fichier envoyer un msg->mtype = 6 
	
	free(msg);
	free(msg1);
	free(msg2);
	free(msg3);
	free(msg4);
	
  	return 0;
}







