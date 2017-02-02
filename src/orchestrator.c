#include "../include/lib_uzzi.h"
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

int main()
{
	pthread_t thread_r; //thread which will lauch REAG PROG
	pthread_t thread_b;	// Thread which will launch BLACK PROG
	pthread_t thread_o;	// Thread which will launch ORCHESTRATOR PROG
	
	CreateIPC_r();	
	CreateIPC_b() ; 
	
	printf("Launch IPC READ and IPC Black \n");
	printf("Launch 3 thread \n");
	
	
	if (pthread_create(&thread_b, NULL, black_thread, NULL) !=0 ) 
	{
		perror("THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
	}
	 
	
	if (pthread_create(&thread_r, NULL , read_thread, NULL) !=0 ) 
		{
			perror("THREAD_ERROR_CREATE");
			return THREAD_ERROR ;	
		}
	
	if (pthread_create(&thread_o, NULL, orche_thread, NULL) !=0 ) 
		{
			perror("THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
		}
	
	if ( ( pthread_join(thread_o, NULL) ) != 0 ) 
		{
			perror("THREAD_ERROR_JOIN");
			return THREAD_ERROR_JOIN ;	
		}
		
	sleep(10);
	CloseIPC_b();
	CloseIPC_r();

	printf("2 IPC were closed \n");
	return 0;
}



