#include "orchestrator.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/* In order to obtain a key */
static key_t doExtractKey_r() {
	return ftok(IPC_NAME,id_read);
}

static ipc_code doOpenIPC_r (int flag) {
	if ( (key_r = doExtractKey_r() ) == -1 )
	{
		return IPC_ERROR;
	}
	if ( (msg_id_r = msgget( key_r, flag ) ) == -1 )  /* Create a message queue */
	{
		return IPC_ERROR;
	}
	return IPC_SUCCESS;
}

ipc_code CreateIPC_r () {
	return doOpenIPC_r( IPC_CREAT | IPC_EXCL | DEFAULT_IPC_PERM_READ ) ;
}


static key_t doExtractKey_b() {
	return ftok(IPC_NAME,id_black);
}

static ipc_code doOpenIPC_b (int flag) {
	if ( (key_b = doExtractKey_b() ) == -1 )
	{
		return IPC_ERROR;
	}
	if ( (msg_id_b = msgget( key_r, flag ) ) == -1 )  /* Create a message queue */
	{
		return IPC_ERROR;
	}
	return IPC_SUCCESS;
}

ipc_code CreateIPC_b() {
	return doOpenIPC_b( IPC_CREAT | IPC_EXCL | DEFAULT_IPC_PERM_BLACK ) ;
}

/* Orchestrator read message from PROG READ  */
ipc_code ReadIPC() {
	mbuf ipcMsg_r;
	//ssize_t len = 0 ; /* length of the message */
	//(void) memset( &ipcMsg, 0, sizeof(mbuf));
	for (;;)
	msgrcv(msg_id_r, &ipcMsg_r, MAX_SIZE, 0, 0);
	printf("Reception de message du PROG READ envoyes par : %s",ipcMsg_r.mtext);
	
}

/* Orchestrator write message for PROG BLACK  */
ipc_code WriteIPC() {
	mbuf ipcMsg_b;
	//ssize_t len = 0 ; /* length of the message */
	ipcMsg_b.mtype = 1;
	ipcMsg_b.mtext[255] = "essai";
	msgsnd(msg_id_b, &ipcMsg_b, MAX_SIZE, 0);
	printf("Envoi vers Blacklist");
	
	return IPC_SUCCESS;
}


int main(int argc, char **argv)
{
	CreateIPC_r();
	CreateIPC_b();
	printf("Orchestrator en attente de message de PROG READ");
	ReadIPC();
	
	
	WriteIPC();
	
	return 0;
	
}
