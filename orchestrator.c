#include "orchestrator.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <bsd/string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <error.h>
#include <errno.h>

/* id message queue for READ PROG*/
static int msg_id_r = 1;

/* id message queue for BLACK PROG*/
static int msg_id_b = 1;

/* In order to obtain the key value from IPC name for READ PROG */
static key_t doExtractKey_r() {
	return ftok(IPC_NAME_READ, id_read);
}

static ipc_code doOpenIPC_r(int flag) {
	if ( (key_r = doExtractKey_r() ) == -1 )
	{
		if (errno == ENOENT ) {
			open("/tmp/squid_data", O_RDWR | O_CREAT | O_APPEND ) ;
			key_r =   doExtractKey_r() ;
							}
		else {
		error(0, errno, "IPC_ERROR_KEY");
		return IPC_ERROR;	}
	}
	if ( (msg_id_r = msgget(key_r, flag ) ) == -1 )  /* Create a message queue */
	{
		if (errno == EEXIST) msg_id_r = msgget(key_r, 0 ) ;
		else {
		error(0, errno, "IPC_ERROR_QUEUE");
		return IPC_ERROR;	}
	}
	return IPC_SUCCESS;
}

ipc_code CreateIPC_r() {
	return doOpenIPC_r(IPC_CREAT  | DEFAULT_IPC_PERM_READ ) ;
}

static ipc_code OpenIPC_r() {
	return doOpenIPC_r(0);
}

static key_t doExtractKey_b() {
	return ftok(IPC_NAME_BLACK, id_black);
}

static ipc_code doOpenIPC_b(int flag) {
	if ( (key_b = doExtractKey_b() ) == -1 )
	{
		if (errno == ENOENT ) {
			open("/tmp/black_data", O_RDWR | O_CREAT | O_APPEND ) ;
			key_b =  doExtractKey_b();
								}
		else {
		error(0, errno, "IPC_ERROR_KEY");
		return IPC_ERROR;	}
	}
	if ( (msg_id_b = msgget(key_b, flag ) ) == -1 )  /* Create a message queue */
	{
		if (errno == EEXIST) msg_id_b = msgget(key_b, 0 ) ;
		else {
		error(0, errno, "IPC_ERROR_QUEUE");
		return IPC_ERROR;	}
	}
	return IPC_SUCCESS;
}

ipc_code CreateIPC_b() {
	return doOpenIPC_b(IPC_CREAT  | DEFAULT_IPC_PERM_BLACK) ;
}

static ipc_code OpenIPC_b() {
	return doOpenIPC_b(0);
}

int CloseIPC_r() {
	return msgctl(msg_id_r, IPC_RMID, NULL);
}

int CloseIPC_b() {
	return msgctl(msg_id_b, IPC_RMID, NULL);
}

/* Orchestrator read message from PROG READ  */
ipc_code ReadIPC() {
	mbuf ipcMsg_r;
	(void) memset( &ipcMsg_r, 0, sizeof(mbuf));
	msgrcv(msg_id_r, (void *) &ipcMsg_r, 255, 0, 0);
	printf("Reception sur orchestrator : %s \n",ipcMsg_r.mtext); 
	CloseIPC_r();
	printf("ReadIPC est ferme \n");
	return IPC_SUCCESS;
}

/* Orchestrator write message for PROG BLACK  */
ipc_code WriteIPC(char *msg) {
	mbuf *ipcMsg_b = NULL;
	ipcMsg_b =  (mbuf * ) malloc(sizeof(mbuf));
	ipcMsg_b->mtype = 42;
	strncpy(ipcMsg_b->mtext,msg,MAX_SIZE);
	msgsnd(msg_id_b, ipcMsg_b,MAX_SIZE,0); 
	printf("%s \n",ipcMsg_b->mtext);
	printf("Send to Blacklist \n");
	sleep(10);
	CloseIPC_b() ;
	return IPC_SUCCESS;
}

/* Thread which launch the reading from READ PROG  */
void *read_thread(void *arg) {
	(void) arg ;
	printf("Open IPC read \n");
	CreateIPC_r() ; 
	printf(" %d \n ",key_r);
	printf("id read %d \n",msg_id_r);
	ReadIPC();
	pthread_exit(NULL);
}

/* Thread which launch the writing from ORCHE PROG towards BLACK PROG  */
void *black_thread(void *arg) {
	(void) arg ;
	printf("Open IPC black \n");
	CreateIPC_b() ; 
	printf("id black %d \n",msg_id_b);
	char *msg_b = "essai";
	WriteIPC(msg_b);
	pthread_exit(NULL);
}

int main()
{
	printf("Avant la creation du Thread \n");
	pthread_t thread_r;
	pthread_t thread_b;
		
	CreateIPC_b() ; 
	printf("Launch IPC Black \n");
		
	if (pthread_create(&thread_r, NULL, read_thread, NULL)  != 0 ) {
			error(0, errno, "THREAD_ERROR_CREATE");
			return THREAD_ERROR ;	}
	
	if ( ( pthread_join(thread_r, NULL) ) != 0 ) {
		error(0, errno, "THREAD_ERROR_JOIN");
		return THREAD_ERROR_JOIN ;	}
		 
	if (pthread_create(&thread_b, NULL, black_thread, NULL) !=0 ) {
			error(0, errno, "THREAD_ERROR_CREATE");
			return THREAD_ERROR;	}
	
	if ( ( pthread_join(thread_b, NULL) ) != 0 ) {
		error(0, errno, "THREAD_ERROR_JOIN");
		return THREAD_ERROR_JOIN ;	}
	
	if ( pthread_detach(&thread_b) != 0 )	{
		perror("pthread_detach() error");
		exit(EXIT_FAILURE);
	}
	if ( pthread_detach(&thread_b) != 0 )	{
		perror("pthread_detach() error");
		exit(EXIT_FAILURE);
	}
	printf("2 IPC were closed \n");
	
	return 0;
}
