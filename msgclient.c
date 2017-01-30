#include "config.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main () {
	  
	printf("Envoi de messages vers orchestrator \n");
	doOpenIPC_r(0);
	mbuf *msg  ;
	msg = (mbuf*)malloc( sizeof(mbuf) ) ;
	mbuf* ipcMsg_r;
	ipcMsg_r = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg->mtype = 42;
	strncpy(msg->mtext, "message0", MAX_SIZE);
	strncpy(msg->autre, " autre champ0", 20);
	msg->ad_IP = 168 ;
	WriteIPC(msg, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	
	sleep(2);
	mbuf *msg1  ;
	msg1 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg1->mtype = 42;
	strncpy(msg1->mtext, "message1", MAX_SIZE);
	strncpy(msg1->autre, " autre champ1", 20);
	msg1->ad_IP = 169 ;
	WriteIPC(msg1, ipcMsg_r, msg_id_r);

	sleep(2);
	mbuf *msg2  ;
	msg2 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg2->mtype = 42;
	strncpy(msg2->mtext, "message2", MAX_SIZE);
	strncpy(msg2->autre, " autre champ2", 20);
	msg2->ad_IP = 170 ;
	WriteIPC(msg2, ipcMsg_r, msg_id_r);
	
	sleep(2);
	mbuf *msg3  ;
	msg3 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg3->mtype = 42;
	strncpy(msg3->mtext, "message3", MAX_SIZE);
	strncpy(msg3->autre, " autre champ3", 20);
	msg3->ad_IP = 171 ;
	WriteIPC(msg3, ipcMsg_r, msg_id_r);
	
	sleep(2);
	mbuf *msg4  ;
	msg4 = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg4->mtype = 42;
	strncpy(msg4->mtext, "message4", MAX_SIZE);
	strncpy(msg4->autre, " autre champ4", 20);
	msg4->ad_IP = 172 ;
	WriteIPC(msg4, ipcMsg_r, msg_id_r);
	
	free(msg);
	free(msg1);
	free(msg2);
	free(msg3);
	free(msg4);
	
  	return 0;
}







