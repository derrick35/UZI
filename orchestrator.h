#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "config.h"


/* Read can send message but only orchestrator can read */
#define DEFAULT_IPC_PERM_READ  0666 

/* Blacklist can read message but only orchestrator can write */
#define DEFAULT_IPC_PERM_BLACK  0666 

#define NB_THREADS 2

enum return_code {THREAD_ERROR, THREAD_ERROR_JOIN } ;

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
ipc_code ReadIPC(char **msg);

/* Orchestrator write message for PROG BLACK  */
ipc_code WriteIPC(char *msg) ;
