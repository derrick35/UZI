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
	change_ids(proxy, uzi);
	printf("Envoi de messages vers orchestrator \n");
	doOpenIPC_r(0);
	
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
	sleep(1);
	
		
	squidLog *msg1  ;
	msg1 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg1->mtype = 42 ; 
	msg1->time = 12345678910 ; 
	strlcpy(msg1->clientIpAdress,"192.168.165.14",LOW_SIZE);
	strlcpy(msg1->urlDest,"students.vassar.edu/vilee",MAX_SIZE);
	strlcpy(msg1->user,"Client2",LOW_SIZE);
	WriteIPC(msg1, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1);
	
	squidLog *msg2  ;
	msg2 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg2->mtype = 42 ; 
	msg2->time = 12345678911 ; 
	strlcpy(msg2->clientIpAdress,"192.168.165.15",LOW_SIZE);
	strlcpy(msg2->urlDest,"www.news.fr",MAX_SIZE);
	strlcpy(msg2->user,"Client3",LOW_SIZE);
	WriteIPC(msg2, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1);
		
	squidLog *msg3  ;
	msg3 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg3->mtype = 42 ; 
	msg3->time = 12345678912 ; 
	strlcpy(msg3->clientIpAdress,"192.168.165.16",LOW_SIZE);
	strlcpy(msg3->urlDest,"www.voila.fr",MAX_SIZE);
	strlcpy(msg3->user,"Client4",LOW_SIZE);
	WriteIPC(msg3, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1);
		
	
	squidLog *msg4  ;
	msg4 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg4->mtype = 42 ; 
	msg4->time = 12345678913 ; 
	strlcpy(msg4->clientIpAdress,"192.168.165.17",LOW_SIZE);
	strlcpy(msg4->urlDest,"www.info.fr",MAX_SIZE);
	strlcpy(msg4->user,"Client5",LOW_SIZE);
	WriteIPC(msg4, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1);
	
	squidLog *msg5  ;
	msg5 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg5->mtype = 42 ; 
	msg5->time = 12345678914 ; 
	strlcpy(msg5->clientIpAdress,"192.168.165.18",LOW_SIZE);
	strlcpy(msg5->urlDest,"1stsecuritybank.com",MAX_SIZE);
	strlcpy(msg5->user,"Client6",LOW_SIZE);
	WriteIPC(msg5, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1);
	
	squidLog *msg6  ;
	msg6 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg6->mtype = 42 ; 
	msg6->time = 12345678915 ; 
	strlcpy(msg6->clientIpAdress,"192.168.165.19",LOW_SIZE);
	strlcpy(msg6->urlDest,"americanfederalsavingsbank.com",MAX_SIZE);
	strlcpy(msg6->user,"Client7",LOW_SIZE);
	WriteIPC(msg6, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1);
	
	squidLog *msg7  ;
	msg7 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg7->mtype = 42 ; 
	msg7->time = 12345678916 ; 
	strlcpy(msg7->clientIpAdress,"192.168.165.20",LOW_SIZE);
	strlcpy(msg7->urlDest,"bankerscompliancegroup.com",MAX_SIZE);
	strlcpy(msg7->user,"Client8",LOW_SIZE);
	WriteIPC(msg7, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1);
	
	squidLog *msg8  ;
	msg8 = (squidLog*)calloc(1, sizeof(squidLog) ) ;
	msg8->mtype = 6 ; 
	msg8->time = 12345678917 ; 
	strlcpy(msg8->clientIpAdress,"192.168.165.21",LOW_SIZE);
	strlcpy(msg8->urlDest,"zeagraff44.skyblog.com/8.html",MAX_SIZE);
	strlcpy(msg8->user,"Client9",LOW_SIZE);
	WriteIPC(msg8, ipcMsg_r, msg_id_r);  //Le message msg est copié et envoyé dans l'IPC READ
	sleep(1); 
	
	printf("fin de l'envoi \n");
	
	//Quand on atteint la fin du fichier envoyer un msg->mtype = 6 
	
	free(msg0);
	free(msg1);
	free(msg2);
	free(msg3);
	free(msg4);
	free(msg5);
	free(msg6);
	free(msg7);
	free(msg8);
	free(ipcMsg_r);
	
  	return 0;
}







