#ifndef _LIB_UZZI_
#define _LIB_UZZI_ 
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/**********  ERROR MANAGEMENT   **********/
typedef enum _IPC_CODE { IPC_ERROR, IPC_SUCCESS } ipc_code;

typedef enum _THREAD_CODE { THREAD_ERROR, THREAD_ERROR_JOIN, THREAD_SUCCESS } thread_code ;


/********** Structure of a message **********/
typedef struct {  
	long mtype;
	char mtext[255]; 	/* type of message */
	char ad_IP[20];		/* IP address */
	char autre[20];		
} mbuf;

/* Programmes configuration for PROG READ, BLACKLIST and ORCHESTRATOR. */

#define MAX_SIZE 255

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_READ "/tmp/squid_data"

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_BLACK "/tmp/black_data"

/* Read can write message but only orchestrator can read */
#define DEFAULT_IPC_PERM_READ  0620 

/* Blacklist can read message but only orchestrator can write */
#define DEFAULT_IPC_PERM_BLACK  0640 


/* id message queue for READ PROG*/
int msg_id_r ;

/* id message queue for BLACK PROG*/
int msg_id_b ;

/* id project for READ PROG */
int id_read ;

/* id project for BLACK PROG */
int id_black ;

/* IPC key for READ PROG */
key_t key_r ;

/* IPC key for BLACK PROG */
key_t key_b ;

/* Functions : */

/* In order to obtain a key */
key_t doExtractKey_r();

/* Error management */
ipc_code doOpenIPC_r(int flag);

/* Obtain a key and create a new message queue with error management */
ipc_code CreateIPC_r() ;

/* In order to obtain a key */
key_t doExtractKey_b();

/* Error management */
ipc_code doOpenIPC_b(int flag);

/* Obtain a key and create a new message queue with error management */
ipc_code CreateIPC_b() ;

/* Orchestrator read message from PROG READ  */
ipc_code ReadIPC(mbuf **msg, mbuf ipcMsg_r, int msg_id);

/* Orchestrator write message for PROG BLACK  */
ipc_code WriteIPC(mbuf *msg, mbuf* ipcMsg_b, int msg_id) ;

/* Definition of strlcpy function from BSD */
size_t strlcpy(char *dst, const char *src, size_t dsize);


void *read_thread(void *arg) ;


void *orche_thread(void *arg);


void *black_thread(void *arg) ;


#endif

