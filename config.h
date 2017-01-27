#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/* Programmes configuration for PROG READ, BLACKLIST and ORCHESTRATOR. */

#define MAX_SIZE 255

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_READ "/tmp/squid_data"

/*  Name of IPC, file must exist and reachable */
#define IPC_NAME_BLACK "/tmp/black_data"

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
	char mtext[255];
	char ad_IP[30];
	char autre[20];
} mbuf;
