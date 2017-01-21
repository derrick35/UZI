#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_SIZE 255

/*  Name of IPC */
#define IPC_NAME "/home/derrick/Documents/CFSSI_2016/PROG /PROG_SECURISEE/Projet/Projet_UZI_local/Projet_UZI_loc/orchestrator.c"

/* Read can send message but only orchestrator can read */
#define DEFAULT_IPC_PERM_READ  0x620 

/* Blacklist can read message but only orchestrator can write */
#define DEFAULT_IPC_PERM_BLACK  0x640 

/* id message queue for READ PROG*/
int msg_id_r = 0;

/* id message queue for BLACK PROG*/
int msg_id_b = 0;

/* id project for READ PROG*/
int id_read = 1;

/* id project for BLACK PROG*/
int id_black = 2;

typedef enum _IPC_CODE { IPC_ERROR, IPC_SUCCESS } ipc_code;

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
