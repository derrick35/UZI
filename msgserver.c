#include "config.h"
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
 
ipc_code Read_IPC( char **msg )	{
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
	
	if ( (len = msgrcv(msg_id_b, & ipcMsg_b, 255, 0, 0)) == -1 )	
		{
		perror("msgcrv failed with error \n");
	    exit(EXIT_FAILURE);
		}
    if ( ( *msg = ( char * ) malloc( len ) ) == NULL ) 
		{ 
			return IPC_ERROR ;  
		}
    (void)memcpy( *msg, ipcMsg_b.mtext, len ) ;
   
        
    return IPC_SUCCESS ; 
}
   
int main () 
	{
	int inprocess = 1;
	char *msg  ;
	while (inprocess) Read_IPC(&msg);
	return 0;
	}

