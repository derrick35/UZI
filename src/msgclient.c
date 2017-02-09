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
	
	printf("Envoi de messages vers orchestrator \n");
	doOpenIPC_r(0);
	printf("test \n");
	squidLog* ipcMsg_r;
	ipcMsg_r = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	
	squidLog *msg0  ;
	msg0 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg0->mtype = 42 ; 
	msg0->time = 123456789 ; 
	strlcpy(msg0->clientIpAdress,"192.168.165.13",LOW_SIZE);
	strlcpy(msg0->urlDest,"1.bp.blogspot.com/_8rixh4qw06m/r9aqpiusgsi/aaaaaaaahpi/bpsm70fvl8o/s1600-h/sexy",MAX_SIZE);
	strlcpy(msg0->user,"Client1",LOW_SIZE);
	WriteIPC(msg0, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(2);
	
		
	squidLog *msg1  ;
	msg1 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg1->mtype = 42 ; 
	msg1->time = 12345678910 ; 
	strlcpy(msg1->clientIpAdress,"192.168.165.14",LOW_SIZE);
	strlcpy(msg1->urlDest,"students.vassar.edu/vilee",MAX_SIZE);
	strlcpy(msg1->user,"Client2",LOW_SIZE);
	WriteIPC(msg1, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(2);
	
	squidLog *msg2  ;
	msg2 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg2->mtype = 42 ; 
	msg2->time = 12345678911 ; 
	strlcpy(msg2->clientIpAdress,"192.168.165.15",LOW_SIZE);
	strlcpy(msg2->urlDest,"www.news.fr",MAX_SIZE);
	strlcpy(msg2->user,"Client3",LOW_SIZE);
	WriteIPC(msg2, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(2);
		
	squidLog *msg3  ;
	msg3 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg3->mtype = 42 ; 
	msg3->time = 12345678912 ; 
	strlcpy(msg3->clientIpAdress,"192.168.165.16",LOW_SIZE);
	strlcpy(msg3->urlDest,"www.voila.fr",MAX_SIZE);
	strlcpy(msg3->user,"Client4",LOW_SIZE);
	WriteIPC(msg3, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(2);
		
	
	squidLog *msg4  ;
	msg4 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg4->mtype = 6 ; 
	msg4->time = 12345678913 ; 
	strlcpy(msg4->clientIpAdress,"192.168.165.17",LOW_SIZE);
	strlcpy(msg4->urlDest,"www.info.fr",MAX_SIZE);
	strlcpy(msg4->user,"Client5",LOW_SIZE);
	WriteIPC(msg4, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(2);
	
	printf("fin de l'envoi \n");
	
	//Quand on atteint la fin du fichier envoyer un msg->mtype = 6 
	
	free(msg0);
	free(msg1);
	free(msg2);
	free(msg3);
	free(msg4);
	free(ipcMsg_r);
	
  	return 0;
}







