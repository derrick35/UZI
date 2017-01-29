#include <stdlib.h>
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


/* Programmes configuration for PROG READ, BLACKLIST and ORCHESTRATOR. */

#define MAX_SIZE 255

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_READ "/tmp/squid_data"

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_BLACK "/tmp/black_data"

/* Read can send message but only orchestrator can read */
#define DEFAULT_IPC_PERM_READ  0666 

/* Blacklist can read message but only orchestrator can write */
#define DEFAULT_IPC_PERM_BLACK  0666 

/* id message queue for READ PROG*/
static int msg_id_r = 1;

/* id message queue for BLACK PROG*/
static int msg_id_b = 1;


/* id project for READ PROG */
int id_read = 46;

/* id project for BLACK PROG */
int id_black = 57;

/* IPC key for READ PROG */
key_t key_r = 0;

/* IPC key for BLACK PROG */
key_t key_b = 0;

typedef enum _IPC_CODE { IPC_ERROR, IPC_SUCCESS } ipc_code;

typedef struct {  //Structure of a message
	long mtype;
	char mtext[255]; /* type of message */
	long ad_IP;
	char autre[20];
} mbuf;


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

int CloseIPC_r() {
	return msgctl(msg_id_r, IPC_RMID, NULL);
}

int CloseIPC_b() {
	return msgctl(msg_id_b, IPC_RMID, NULL);
}

/* Orchestrator read message from IPC_r  */
ipc_code ReadIPC(mbuf ** msg, mbuf ipcMsg, int msg_id) 
{
	//mbuf ipcMsg_r;
	ssize_t len = 0 ;  //length of the message
	(void) memset( &ipcMsg, 0, sizeof(mbuf));
	if (msg == NULL) 
	{ 
		perror("IPC_ERROR") ; 
		exit(EXIT_FAILURE); 
	}
	if ( *msg == NULL) 
	{ 
		perror("IPC_ERROR1") ; 
		exit(EXIT_FAILURE);  
	}
	if ( (len = msgrcv(msg_id, (void *) &ipcMsg, sizeof(mbuf) +1, 0, 0) ) == -1) {return IPC_ERROR ; }
	if ((*msg = (mbuf * ) malloc (len ) ) == NULL) {error(0, errno,"malloc_error"); }
	(void)memmove( *msg, &ipcMsg, len ) ;
	strncpy((*msg)->mtext,ipcMsg.mtext,MAX_SIZE);
	(*msg)->ad_IP = ipcMsg.ad_IP ; 
	strncpy((*msg)->autre,ipcMsg.autre,20);
	
	return IPC_SUCCESS;
}

/* Orchestrator write message for IPC_b  */
ipc_code WriteIPC(mbuf *msg, mbuf *ipcMsg, int msg_id) {
	//mbuf *ipcMsg = NULL;
	if (  (ipcMsg =  (mbuf * ) malloc(sizeof(mbuf)) ) == NULL )
	{
		return IPC_ERROR ;
	}
	ipcMsg->mtype = msg->mtype ;
	strncpy(ipcMsg->mtext,msg->mtext,MAX_SIZE);
	ipcMsg->ad_IP = msg->ad_IP;
	strncpy(ipcMsg->autre,msg->autre,20);
	if ( (msgsnd(msg_id, ipcMsg, sizeof(mbuf) ,0)) == -1 )
	{
		return IPC_ERROR ; 
	} 
	return IPC_SUCCESS;
}

