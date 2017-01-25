#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_SIZE 255

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_READ "/tmp/squid_data"

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_BLACK "/tmp/black_data"

/* Read can send message but only orchestrator can read */
#define DEFAULT_IPC_PERM_READ  0666 

/* Blacklist can read message but only orchestrator can write */
#define DEFAULT_IPC_PERM_BLACK  0666 

#define NB_THREADS 2

typedef enum _IPC_CODE { IPC_ERROR, IPC_SUCCESS } ipc_code;

typedef enum {THREAD_ERROR, THREAD_ERROR_JOIN } ;

/* id project for READ PROG */
int id_read = 46;

/* id project for BLACK PROG */
int id_black = 57;

/* IPC key for READ PROG */
key_t key_r = 0;

/* IPC key for BLACK PROG */
key_t key_b = 0;


typedef struct {  //Structure of a message
	long mtype;
	char mtext[255];
} mbuf;

/* Functions : */

/* In order to obtain a key */
static key_t doExtractKey_r();

/* Error management */
static ipc_code doOpenIPC_r(int flag);

/* Obtain a key and create a new message queue with error management */
ipc_code CreateIPC_r() ;

/* In order to obtain a key */
static key_t doExtractKey_b();

/* Error management */
static ipc_code doOpenIPC_b(int flag);

/* Obtain a key and create a new message queue with error management */
ipc_code CreateIPC_b() ;

/* Orchestrator read message from PROG READ  */
ipc_code ReadIPC();

/* Orchestrator write message for PROG BLACK  */
ipc_code WriteIPC(char *msg) ;
