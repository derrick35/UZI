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


/* ********************************************************** */
/* *************** Initialisation of variables ************** */
/* ********************************************************** */

/* id message queue for READ PROG*/
int msg_id_r = 1 ;

/* id message queue for BLACK PROG*/
int msg_id_b = 1 ;

/* id project for READ PROG */
const int id_read = 46;

/* id project for BLACK PROG */
const int id_black = 57 ;

/* IPC key for READ PROG */
key_t key_r = 0;

/* IPC key for BLACK PROG */
key_t key_b = 0 ;


/* ****************************************************************************************** */
/* ***************In order to obtain the key value from IPC name for READ PROG ************** */
/* ****************************************************************************************** */
key_t doExtractKey_r() 
{
	return ftok(IPC_NAME_READ, id_read);
}

/* *********************************************************************** */
/* ***************In order to open a IPC for READ Programme ************** */
/* *********************************************************************** */
ipc_code doOpenIPC_r(int flag) 
{
	if ( (key_r = doExtractKey_r() ) == -1 )
	{
		if (errno == ENOENT ) 
		{
			open(IPC_NAME_READ, O_RDWR | O_CREAT | O_APPEND ) ;
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
		if (errno == EEXIST)
		{ 
			msg_id_r = msgget(key_r, 0 ) ;
		}
		else 
		{
			perror("IPC_ERROR_QUEUE");
			return IPC_ERROR;	
		}
	}
	return IPC_SUCCESS;
}

/* ************************************************************* */
/* *************** Create  IPC for READ Programme ************** */
/* ************************************************************* */
ipc_code CreateIPC_r() 
{
	return doOpenIPC_r(IPC_CREAT  | DEFAULT_IPC_PERM_READ ) ;
}

/* ************************************************************************************************** */
/* *************** In order to obtain the key value from IPC name for BLACK Programme  ************** */
/* ************************************************************************************************** */
key_t doExtractKey_b() 
{
	return ftok(IPC_NAME_BLACK, id_black);
}

/* ************************************************************************* */
/* *************** In order to open a IPC for BLACK Programme ************** */
/* ************************************************************************* */
ipc_code doOpenIPC_b(int flag) 
{
	if ( (key_b = doExtractKey_b() ) == -1 )
	{
		if (errno == ENOENT ) 
			{
				open(IPC_NAME_BLACK, O_RDWR | O_CREAT | O_APPEND ) ;
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

/* ************************************************************** */
/* *************** Create  IPC for BLACK Programme ************** */
/* ************************************************************** */
ipc_code CreateIPC_b() 
{
	return doOpenIPC_b(IPC_CREAT  | DEFAULT_IPC_PERM_BLACK) ;
}

/* ******************************************** */
/* *************** Close the IPC ************** */
/* ******************************************** */
int CloseIPC_r() 
{
	return msgctl(msg_id_r, IPC_RMID, NULL);
}

int CloseIPC_b() 
{
	return msgctl(msg_id_b, IPC_RMID, NULL);
}

/* ********************************************************************* */
/* ***************  Orchestrator read message from IPC  ************** */
/* ********************************************************************* */
/* Data from IPC are written in the message msg */
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
			perror("IPC_ERROR") ; 
			exit(EXIT_FAILURE);  
		}
	if ( (len = msgrcv(msg_id, (void *) &ipcMsg, sizeof(squidLog) +1, 0, 0) ) == -1) 
		{
			return IPC_ERROR ;
		}
	if ((*msg = (squidLog * ) calloc (1,len ) ) == NULL) 
		{
			perror("calloc_error");
			exit(EXIT_FAILURE); 
		}
	
	(void)memmove( *msg, &ipcMsg, len ) ;
	(*msg)->mtype = ipcMsg.mtype ; 
	(*msg)->time = ipcMsg.time ; 
	strlcpy((*msg)->clientIpAdress,ipcMsg.clientIpAdress,LOW_SIZE);
	strlcpy((*msg)->urlDest,ipcMsg.urlDest,MAX_SIZE);
	strlcpy((*msg)->user,ipcMsg.user,LOW_SIZE);
	
	
	return IPC_SUCCESS;
}

/* ********************************************************************* */
/* ***************  Orchestrator write message for IPC  ************** */
/* ********************************************************************* */
/* Data are written and sent to IPC */
ipc_code WriteIPC(squidLog *msg, squidLog *ipcMsg, int msg_id) 
{
	if (  (ipcMsg =  (squidLog * ) calloc(1,sizeof(squidLog)) ) == NULL )
		{
			return IPC_ERROR ;
		}
	if (  msg == NULL )
		{
			return IPC_ERROR ;
		}
	
	
	ipcMsg->mtype = msg->mtype ;
	ipcMsg->time = msg->time ;
	strlcpy(ipcMsg->clientIpAdress,msg->clientIpAdress,LOW_SIZE);
	strlcpy(ipcMsg->urlDest,msg->urlDest, MAX_SIZE);
	strlcpy(ipcMsg->user,msg->user,LOW_SIZE);
	
	if ( (msgsnd(msg_id, ipcMsg, sizeof(squidLog) +1 ,0)) == -1 )
		{
			return IPC_ERROR ; 
		} 
	
	return IPC_SUCCESS;
}

/* ********************************************************************* */
/* ***************  Function from BSD environment ************** */
/* ********************************************************************* */
size_t strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;
	/*
	if ( (osrc = (char *)calloc( 1, sizeof(char))) == NULL)
		{
			perror("calloc");
			free(osrc);
			exit(EXIT_FAILURE);
		}
		
	if ( (dst = (char *)calloc( 1, sizeof(char))) == NULL)
		{
			perror("calloc");
			free(dst);
			exit(EXIT_FAILURE);
		}	*/

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

/* *********************************************************************** */
/* ***************  Thread which launch the READ Programme  ************** */
/* *********************************************************************** */
void *read_thread(void *arg) 
{
	(void) arg ;	//in order to have no warning
	printf("Begin READ \n");
	FILE *fr = popen(PROG_READ, "r" ) ; 
	if ( fr == NULL )
		{
			perror("popen_read_error");
			exit(EXIT_FAILURE);
		}
	if ( pclose(fr) == -1) 
		{ 
			perror("pclose_error");
			exit(EXIT_FAILURE);
		}
	pthread_exit(NULL);
	
}

/* ********************************************************************************* */
/* ***************  Thread which read from IPC_r and write on IPC_b   ************** */
/* ********************************************************************************* */
void *orche_thread(void *arg) 
{
	(void) arg ;	//in order to have no warning
	squidLog *msg  ;
	squidLog ipcMsg_r;
	squidLog *ipcMsg_b;
	
	if ( (ipcMsg_b = (squidLog*)calloc( 1 , sizeof(squidLog)) ) == NULL )
		{
			perror("THREAD_ERROR_ORCHE_CALLOC");
			exit(EXIT_FAILURE);
		} 	 ;
	if ( (msg = (squidLog*)calloc(1 , sizeof(squidLog) ) ) == NULL )
		{
			perror("THREAD_ERROR_ORCHE_CALLOC");
			exit(EXIT_FAILURE);
		} 
	
	 printf("Begin orche \n");
	
	 do
		{
			ReadIPC(&msg, ipcMsg_r, msg_id_r);
			printf("Reception sur orchestrator : %s / %s / %s \n",msg->clientIpAdress, msg->urlDest, msg->user); 
			WriteIPC(msg, ipcMsg_b, msg_id_b);	
					
		} while (msg->mtype != 6); // no message to read if mtype = 6
	
	free(msg);
	free(ipcMsg_b);
	pthread_exit(NULL);
	
}

/* ************************************************************************ */
/* ***************  Thread which launch the BLACK Programme  ************** */
/* ************************************************************************ */
void *black_thread(void *arg) 
{
	(void) arg ;   //in order to have no warning
	printf("Begin BLACK \n");
	char *lu;
	FILE *fb = popen(PROG_BLACK, "r" ) ;
	
	if ( fb == NULL )
	{
		perror("popen_black_error");
		exit(EXIT_FAILURE);
	}
	
	if ( (lu = (char *)calloc(MAX, sizeof(char)) ) == NULL )
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	
	while ( fgets(lu, MAX, fb) != NULL ) 
		{
			fputs(lu, stdout); 
		}
	
	if ( pclose(fb) == -1 ) 
	{ 
		perror("pclose_error"); 
		exit(EXIT_FAILURE);
	}
	
	pthread_exit(NULL);
}
