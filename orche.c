#include "orche.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <bsd/string.h>
#include <errno.h>

/* id message queue for READ PROG*/
static int msg_id_r = 0;

/* id message queue for BLACK PROG*/
static int msg_id_b = 0;

/* In order to obtain the key value from IPC name for READ PROG */
static key_t doExtractKey_r() {
	return ftok(IPC_NAME,id_read);
}

static ipc_code doOpenIPC_r(int flag) {
	if ( (key_r = doExtractKey_r() ) == -1 )
	{
		error(0, errno, "IPC_ERROR_KEY");
		return IPC_ERROR;
	}
	if ( (msg_id_r = msgget(key_r, flag ) ) == -1 )  /* Create a message queue */
	{
		error(0, errno, "IPC_ERROR_QUEUE1");
		return IPC_ERROR;
	}
	return IPC_SUCCESS;
}

ipc_code CreateIPC_r() {
	return doOpenIPC_r(IPC_CREAT | IPC_EXCL | DEFAULT_IPC_PERM_READ ) ;
}

static ipc_code OpenIPC_r() {
	return doOpenIPC_r(0);
}

static key_t doExtractKey_b() {
	return ftok(IPC_NAME,id_black);
}

static ipc_code doOpenIPC_b(int flag) {
	if ( (key_b = doExtractKey_b() ) == -1 )
	{
		error(0, errno, "IPC_ERROR_KEY");
		return IPC_ERROR;
	}
	if ( (msg_id_b = msgget( key_r, flag ) ) == -1 )  /* Create a message queue */
	{
		error(0, errno, "IPC_ERROR_QUEUE");
		return IPC_ERROR;
	}
	return IPC_SUCCESS;
}

ipc_code CreateIPC_b() {
	return doOpenIPC_b(IPC_CREAT | IPC_EXCL | DEFAULT_IPC_PERM_BLACK) ;
}

static ipc_code OpenIPC_b() {
	return doOpenIPC_b(0);
}

/* Orchestrator read message from PROG READ  */
ipc_code ReadIPC(char **msg) {
	mbuf ipcMsg_r;
	ssize_t len = 0 ; /* length of the message */
	(void) memset( &ipcMsg_r, 0, sizeof(mbuf));
	printf("debug");
	for (;;) {
	//long size = sizeof(ipcMsg_r);
	len = msgrcv(msg_id_r, &ipcMsg_r, 255, 0, 0);
	*msg = (char *) malloc(len);
	(void) memcpy(*msg, ipcMsg_r.mtext, len);
	printf("Reception de message du PROG READ envoyes par : %s \n",ipcMsg_r.mtext);}
}

/* Orchestrator write message for PROG BLACK  */
ipc_code WriteIPC(char *msg) {
	mbuf *ipcMsg_b = NULL;
	ipcMsg_b =  (mbuf * ) malloc(sizeof(mbuf));
	ipcMsg_b->mtype = 42;
	//long size = sizeof(ipcMsg_b);
	strncpy(ipcMsg_b->mtext,msg,255);
	//strncpy(ipcMsg_b->mtext, msg, size);
	printf("debug1 \n");
	int send = msgsnd(msg_id_b, ipcMsg_b, strlen( ipcMsg_b->mtext ) + 1 , 0);
	printf("%d \n",send);
	printf("Envoi vers Blacklist \n");
	return IPC_SUCCESS;
}

int main()
{
	char **msg_r = NULL;
	char *msg_b = "essai";
	OpenIPC_r();
	OpenIPC_b();
	printf("les 2 IPC ont ete ouvertes\n");
	printf("Orchestrator en attente de message de PROG READ \n");
	WriteIPC(msg_b);
	printf("entre les 2");
	ReadIPC(msg_r);
	printf("avant sleep");
	sleep(10);
	printf("apres sleep");
	msgctl(msg_id_r, IPC_RMID, NULL);
	msgctl(msg_id_b, IPC_RMID, NULL);
	printf("Les 2 IPC ont été fermees \n");
	
	return 0;
}
