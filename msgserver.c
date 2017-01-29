#include "config.h"
//#include "orchestrator.h"  // A enlever après la phase de test
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>
 
/*
ipc_code Read_IPC( mbuf **msg )	{
	mbuf	ipcMsg_b;
	ssize_t len = 0;	//length of the message
	memset( &ipcMsg_b, 0, sizeof( mbuf ) ) ;
	if (msg == NULL) 
		{ 
		perror("IPC_ERROR2") ; 
		exit(EXIT_FAILURE); 
		}
	if ( *msg == NULL) 
		{ 
		perror("IPC_ERROR21") ; 
		exit(EXIT_FAILURE);  
		}
	key_b = ftok(IPC_NAME_BLACK,id_black);
	int msg_id_b = msgget( key_b, 0 ) ;
	
	if ( (len = msgrcv(msg_id_b, (void *) & ipcMsg_b, sizeof(mbuf)+1, 0, 0)) == -1 )	
		{
		perror("End lecture on IPC_b / msgcrv failed with error \n");
	    exit(EXIT_FAILURE);
		}
    if ( ( *msg = ( mbuf * ) malloc( len ) ) == NULL ) 
		{ 
			error(0, errno,"malloc_error");  
		}
    (void)memmove( *msg, &ipcMsg_b, len ) ;
    strncpy((*msg)->mtext,ipcMsg_b.mtext,MAX_SIZE);
	(*msg)->ad_IP = ipcMsg_b.ad_IP ; 
	strncpy((*msg)->autre,ipcMsg_b.autre,20);
	printf("Lecture par blacklist : %s / %ld / %s \n",(*msg)->mtext, (*msg)->ad_IP, (*msg)->autre); 
       
    return IPC_SUCCESS ; 
}	*/
   
int main () 
	{
	int inprocess = 1;
	doOpenIPC_b(0);
	mbuf *msg  ;
	msg = (mbuf*)malloc( sizeof(mbuf) ) ;
	mbuf ipcMsg_b;
	while (inprocess)
	{ ReadIPC(&msg, ipcMsg_b, msg_id_b); //  Les données dans l'IPC BLACK sont copiées dans le message msg
	printf("Lecture par blacklist : %s / %ld / %s \n",msg->mtext, msg->ad_IP, msg->autre); 
	}
	free(msg);
	return 0;
	}

