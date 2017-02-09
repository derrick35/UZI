#ifndef _LIB_UZZI_
#define _LIB_UZZI_ 
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/* *************************************************** */
/* **************  ERROR MANAGEMENT   *************** */
/* *************************************************** */
typedef enum _IPC_CODE { IPC_ERROR, IPC_ERROR_R, IPC_ERROR_W, IPC_SUCCESS } ipc_code;

typedef enum _THREAD_CODE { THREAD_ERROR, THREAD_ERROR_JOIN, THREAD_SUCCESS } thread_code ;


/* *********************************************************************** */
/* Programmes configuration for PROG READ, BLACKLIST and ORCHESTRATOR. */
/* ********************************************************************** */

#define MAX 1024

#define MAX_SIZE 255

#define MIDDLE_SIZE 50

#define LOW_SIZE 25

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_READ "/tmp/squid_data"

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_BLACK "/tmp/black_data"

/* Name of the READ executable */
#define PROG_READ "./msgclient"

/* Name of the BLACK executable */
#define PROG_BLACK "./msgserver"

/* Name of the Blacklist file */
#define FILE_BLACK "blacklist.txt"

/* Read can write message but only orchestrator can read */
#define DEFAULT_IPC_PERM_READ  0622 

/* Blacklist can read message but only orchestrator can write */
#define DEFAULT_IPC_PERM_BLACK  0644


/* ********************************************* */
/* Name of the variables used by the programmes */
/* ********************************************* */

/* id message queue for READ PROG*/
int msg_id_r ;

/* id message queue for BLACK PROG*/
int msg_id_b ;

/* id project for READ PROG */
const int id_read ;

/* id project for BLACK PROG */
const int id_black ;

/* IPC key for READ PROG */
key_t key_r ;

/* IPC key for BLACK PROG */
key_t key_b ;


/* ***************************************** */
/* ********* Structure of a message **********/
/* ***************************************** */
typedef struct {  
	long 	mtype;
	long 	time ;
	long 	duration;
	char 	clientIpAdress[LOW_SIZE];
	char 	resultCodes[MIDDLE_SIZE];
	int 	bytes;
	char 	method[LOW_SIZE] ; 
	char 	urlDest[MAX_SIZE]; 			/* type of message */
	char 	user[LOW_SIZE];				/* IP address */
	char 	hostCode[MIDDLE_SIZE];	
	char	type[MIDDLE_SIZE];	
} squidLog;


/* ****************************/
/* ******* Functions ******** */
/* ************************** */

/* In order to obtain a key */
key_t doExtractKey_r();

/* OPen IPC for READ PROG */
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
ipc_code ReadIPC(squidLog **msg, squidLog ipcMsg_r, int msg_id);

/* Orchestrator write message for PROG BLACK  */
ipc_code WriteIPC(squidLog *msg, squidLog* ipcMsg_b, int msg_id) ;

/* Definition of strlcpy function from BSD */
size_t strlcpy(char *dst, const char *src, size_t dsize);

/* Launch Thread of the READ PROG */
void *read_thread(void *arg) ;

/* Launch Thread of the ORCHESTRATOR PROG */
void *orche_thread(void *arg);

/* Launch Thread of the BLACK PROG */
void *black_thread(void *arg) ;


#endif

