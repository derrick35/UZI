#include "../include/lib_uzzi.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <error.h>
#include <errno.h>

/* id message queue for READ PROG*/
int msg_id_r = 1 ;

/* id message queue for BLACK PROG*/
int msg_id_b = 1 ;

/* id project for READ PROG */
int id_read = 46;

/* id project for BLACK PROG */
int id_black = 57 ;

/* IPC key for READ PROG */
key_t key_r = 0;

/* IPC key for BLACK PROG */
key_t key_b = 0 ;


/********** In order to obtain the key value from IPC name for READ PROG **********/
key_t doExtractKey_r() 
{
	return ftok(IPC_NAME_READ, id_read);
}

/********** In order to open a IPC for READ Programme **********/
ipc_code doOpenIPC_r(int flag) 
{
	if ( (key_r = doExtractKey_r() ) == -1 )
	{
		if (errno == ENOENT ) 
		{
			open("/tmp/squid_data", O_RDWR | O_CREAT | O_APPEND ) ;
			key_r =   doExtractKey_r() ;
		}
		else 
		{
			perror("IPC_ERROR_KEY");
			return IPC_ERROR;	
		}
	}
	if ( (msg_id_r = msgget(key_r, flag ) ) == -1 )  /* Create a message queue */
	{
		if (errno == EEXIST) msg_id_r = msgget(key_r, 0 ) ;
		else 
		{
			perror("IPC_ERROR_QUEUE");
			return IPC_ERROR;	
		}
	}
	return IPC_SUCCESS;
}

/**********   Create  IPC for READ Programme**********/
ipc_code CreateIPC_r() 
{
	return doOpenIPC_r(IPC_CREAT  | DEFAULT_IPC_PERM_READ ) ;
}

/********** In order to obtain the key value from IPC name for BLACK Programme **********/
key_t doExtractKey_b() 
{
	return ftok(IPC_NAME_BLACK, id_black);
}

/********** In order to open a IPC for BLACK Programme **********/
ipc_code doOpenIPC_b(int flag) 
{
	if ( (key_b = doExtractKey_b() ) == -1 )
	{
		if (errno == ENOENT ) 
			{
				open("/tmp/black_data", O_RDWR | O_CREAT | O_APPEND ) ;
				key_b =  doExtractKey_b();
			}
		else 
			{
				perror("IPC_ERROR_KEY");
				return IPC_ERROR;	
			}
	}
	if ( (msg_id_b = msgget(key_b, flag ) ) == -1 )  /* Create a message queue */
		{
			if (errno == EEXIST) 
				{
					msg_id_b = msgget(key_b, 0 ) ;
				}
			else 
				{
					perror("IPC_ERROR_QUEUE");
					return IPC_ERROR;	
				}
		}
	return IPC_SUCCESS;
}

/**********   Create  IPC for READ Programme**********/
ipc_code CreateIPC_b() 
{
	return doOpenIPC_b(IPC_CREAT  | DEFAULT_IPC_PERM_BLACK) ;
}

/**********   Close the IPC  *********/
int CloseIPC_r() 
{
	return msgctl(msg_id_r, IPC_RMID, NULL);
}

int CloseIPC_b() 
{
	return msgctl(msg_id_b, IPC_RMID, NULL);
}

/********** Orchestrator read message from IPC_r  **********/
//  Les données dans l'IPC sont copiées dans le message msg
ipc_code ReadIPC(squidLog ** msg, squidLog ipcMsg, int msg_id) 
{
	ssize_t len = 0 ;  //length of the message
	(void) memset( &ipcMsg, 0, sizeof(squidLog));
	
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
	if ( (len = msgrcv(msg_id, (void *) &ipcMsg, sizeof(squidLog) +1, 0, 0) ) == -1) 
		{
			return IPC_ERROR ;
		}
	if ((*msg = (squidLog * ) malloc (len ) ) == NULL) 
		{
			perror("malloc_error"); 
		}
	
	(void)memmove( *msg, &ipcMsg, len ) ;
	(*msg)->mtype = ipcMsg.mtype ; 
	(*msg)->time = ipcMsg.time ; 
	strlcpy((*msg)->clientIpAdress,ipcMsg.clientIpAdress,LOW_SIZE);
	strlcpy((*msg)->urlDest,ipcMsg.urlDest,MAX_SIZE);
	strlcpy((*msg)->user,ipcMsg.user,LOW_SIZE);
	
	
	return IPC_SUCCESS;
}

/********** Orchestrator write message for IPC_b  **********/
//Le message msg est copié et envoyé dans l'IPC 
ipc_code WriteIPC(squidLog *msg, squidLog *ipcMsg, int msg_id) 
{
	if ( msg == NULL )
		{
			return IPC_ERROR;
		}
	if (  (ipcMsg =  (squidLog * ) malloc(sizeof(squidLog)) ) == NULL )
		{
			return IPC_ERROR ;
		}
	
	ipcMsg->mtype = msg->mtype ;
	ipcMsg->time = msg->time ;
	strlcpy(ipcMsg->clientIpAdress,msg->clientIpAdress,LOW_SIZE);
	strlcpy(ipcMsg->urlDest,msg->urlDest, MAX_SIZE);
	strlcpy(ipcMsg->user,msg->user,20);
	
	if ( (msgsnd(msg_id, ipcMsg, sizeof(squidLog) +1 ,0)) == -1 )
		{
			return IPC_ERROR ; 
		} 
	
	return IPC_SUCCESS;
}


size_t
strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';		/* NUL-terminate dst */
		while (*src++)
			;
	}

	return(src - osrc - 1);	/* count does not include NUL */
}


/* Thread which launch the msgclient PROG  */
void *read_thread(void *arg) 
{
	(void) arg ;
	printf("Begin READ \n");
	FILE *fr = popen("./msgclient", "r" ) ; 
	if ( fr == NULL )
		{
			perror("THREAD_ERROR_READ");
			exit(EXIT_FAILURE);
		}
	if ( pclose(fr) == -1) 
		{ 
			perror("pclose_error");
		}
	pthread_exit(NULL);
	
}

/* Thread which read from IPC_r and write on IPC_b */

void *orche_thread(void *arg) 
{
	(void) arg ;
	squidLog *msg  ;
	squidLog ipcMsg_r;
	squidLog *ipcMsg_b;
	
	if ( (ipcMsg_b = (squidLog*)malloc( sizeof(squidLog)) ) == NULL )
		{
			perror("THREAD_ERROR_ORCHE_MALLOC");
			exit(EXIT_FAILURE);
		} 	 ;
	if ( (msg = (squidLog*)malloc( sizeof(squidLog) ) ) == NULL )
		{
			perror("THREAD_ERROR_ORCHE_MALLOC");
			exit(EXIT_FAILURE);
		} 
	
	 printf("Begin orche \n");
	 //int i ;
	 do
	 {
	//for (i = 0; i < 5 ; i = i+1 ) 
		//{
			ReadIPC(&msg, ipcMsg_r, msg_id_r);
			printf("Reception sur orchestrator : %s / %s / %s \n",msg->clientIpAdress, msg->urlDest, msg->user); 
			WriteIPC(msg, ipcMsg_b, msg_id_b);	
			
		//}
	} while (msg->mtype != 6); //Un mtype égal à 6 indique qu'il n'y a plus de message à lire
	
	free(msg);
	free(ipcMsg_b);
	pthread_exit(NULL);
	
}

/* Thread which launch the msgserver PROG  */
void *black_thread(void *arg) 
{
	(void) arg ;
	printf("Begin BLACK \n");
	FILE *fb = popen("./msgserver", "r" ) ;
	if ( fb == NULL )
	{
		perror("THREAD_ERROR_BLACK");
		exit(EXIT_FAILURE);
	}
	if ( pclose(fb) == -1 ) { perror("pclose_error"); }
	
	pthread_exit(NULL);
}
