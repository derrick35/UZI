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


/* Thread which launch the msgclient PROG  */
void *read_thread(void *arg) 
{
	(void) arg ;
	printf("Begin to send to IPC_r \n");
	FILE *fr = popen("./msgclient", "r" ) ; 
	if ( fr == NULL )
	{
		error(0, errno, "THREAD_ERROR_READ");
		exit(EXIT_FAILURE);
	}
	if ( pclose(fr) == -1) { perror("pclose_error"); }
	pthread_exit(NULL);
}

/* Thread which read from IPC_r and write on IPC_b */
void *orche_thread(void *arg) 
{
	(void) arg ;
	mbuf *msg  ;
	mbuf ipcMsg_r;
	mbuf *ipcMsg_b;
	ipcMsg_b = (mbuf*)malloc( sizeof(mbuf) ) ;
	msg = (mbuf*)malloc( sizeof(mbuf) ) ;
	int i ;
	for (i = 0; i < 5 ; i = i+1 ) 
	{
		ReadIPC(&msg, ipcMsg_r, msg_id_r);
		printf("Reception sur orchestrator : %s / %ld / %s \n",msg->mtext, msg->ad_IP, msg->autre); 
		WriteIPC(msg, ipcMsg_b, msg_id_b);	
		printf("Send to IPC_b : %s / %ld / %s \n", ipcMsg_b->mtext, ipcMsg_b->ad_IP, ipcMsg_b->autre);
	}
	free(msg);
	pthread_exit(NULL);
}

/* Thread which launch the msgserver PROG  */
void *black_thread(void *arg) 
{
	(void) arg ;
	printf("Begin reception on Blacklist \n");
	FILE *fb = popen("./msgserver", "r" ) ;
	if ( fb == NULL )
	{
		error(0, errno, "THREAD_ERROR_BLACK");
		exit(EXIT_FAILURE);
	}
	if ( pclose(fb) == -1 ) { perror("pclose_error"); }
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread_r;
	pthread_t thread_b;
	pthread_t thread_o;
	
	CreateIPC_r();	
	CreateIPC_b() ; 
	
	printf("Launch IPC READ and IPC Black \n");
	printf("Launch 3 thread \n");
	
	if (pthread_create(&thread_b, NULL, black_thread, NULL) !=0 ) 
	{
		error(0, errno, "THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
	}
	
	if (pthread_create(&thread_r, NULL , read_thread, NULL) !=0 ) 
	{
		error(0, errno, "THREAD_ERROR_CREATE");
		return THREAD_ERROR ;	
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
		
	sleep(15);
	CloseIPC_b();
	CloseIPC_r();

	printf("2 IPC were closed \n");
	
	return 0;
}



