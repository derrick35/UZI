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
ipc_code ReadIPC(char ** msg) 
{
	mbuf ipcMsg_r;
	ssize_t len = 0 ;  //length of the message
	(void) memset( &ipcMsg_r, 0, sizeof(mbuf));
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
	len = msgrcv(msg_id_r, (void *) &ipcMsg_r, 255, 0, 0);
	*msg = (char * ) malloc (len ) ;
	(void)memcpy( *msg, ipcMsg_r.mtext, len ) ;
	printf("Reception sur orchestrator : %s \n",*msg); 
	return IPC_SUCCESS;
}

/* Orchestrator write message for IPC_b  */
ipc_code WriteIPC(char *msg) {
	mbuf *ipcMsg_b = NULL;
	ipcMsg_b =  (mbuf * ) malloc(sizeof(mbuf));
	ipcMsg_b->mtype = 42;
	strncpy(ipcMsg_b->mtext,msg,MAX_SIZE);
	msgsnd(msg_id_b, ipcMsg_b,MAX_SIZE,0); 
	printf("Send to Blacklist : %s \n", ipcMsg_b->mtext);
	return IPC_SUCCESS;
}

/* Thread which launch the msgclient PROG  */
void *read_thread(void *arg) 
{
	(void) arg ;
	if ( popen("./msgclient", "r" ) == NULL )
	{
		error(0, errno, "THREAD_ERROR_READ");
		exit(EXIT_FAILURE);
	}
	pthread_exit(NULL);
}

/* Thread which read from IPC_r and write on IPC_b */
void *orche_thread(void *arg) 
{
	(void) arg ;
	char *msg = "test" ;
	int i ;
	for (i = 0; i < 6 ; i = i+1 ) 
	{
		ReadIPC(&msg);
		WriteIPC(msg);	
	}
	pthread_exit(NULL);
}

/* Thread which launch the msgserver PROG  */
void *black_thread(void *arg) 
{
	(void) arg ;
	if ( popen("./msgserver", "r" ) == NULL )
	{
		error(0, errno, "THREAD_ERROR_BLACK");
		exit(EXIT_FAILURE);
	}
	pthread_exit(NULL);
}

int main()
{
	printf("Avant la creation du Thread \n");
	pthread_t thread_r;
	pthread_t thread_b;
	pthread_t thread_o;
	
	CreateIPC_r();	
	CreateIPC_b() ; 
	printf("Launch IPC READ and IPC Black \n");
	
	if (pthread_create(&thread_r, NULL , read_thread, NULL) !=0 ) 
	{
		error(0, errno, "THREAD_ERROR_CREATE");
		return THREAD_ERROR ;	
	}
	
	if (pthread_create(&thread_b, NULL, black_thread, NULL) !=0 ) 
	{
		error(0, errno, "THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
	}
	
	if (pthread_create(&thread_o, NULL, orche_thread, NULL) !=0 ) 
	{
		error(0, errno, "THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
	}
	
	if ( ( pthread_join(thread_o, NULL) ) != 0 ) 
	{
		error(0, errno, "THREAD_ERROR_JOIN");
		return THREAD_ERROR_JOIN ;	
	}
		
	
	sleep(30);
	CloseIPC_b();
	CloseIPC_r();

	printf("2 IPC were closed \n");
	
	return 0;
}



