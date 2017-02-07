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
	pthread_t thread_r; /* identify thread who will launch REAG PROG */
	pthread_t thread_b;	/* identify thread who will launch BLACK PROG */
	pthread_t thread_o;	/* identify thread who will launch ORCHESTRATOR PROG */
	pthread_attr_t thread_attr_b;
	pthread_attr_t thread_attr_r;
	
	
	CreateIPC_r();	
	CreateIPC_b() ; 
	
	printf("Launch IPC READ and IPC Black \n");
	printf("Launch 3 thread \n");
	
	if (pthread_attr_init(&thread_attr_b) != 0)
	{
		perror("pthread_attr_init error");
		exit(EXIT_FAILURE);
	}
	if (pthread_attr_setdetachstate(&thread_attr_b, PTHREAD_CREATE_DETACHED) != 0)
	{
		perror("pthread_attr_setdetachstate error");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_attr_init(&thread_attr_r) != 0)
	{
		perror("pthread_attr_init error");
		exit(EXIT_FAILURE);
	}
	if (pthread_attr_setdetachstate(&thread_attr_r, PTHREAD_CREATE_DETACHED) != 0)
	{
		perror("pthread_attr_setdetachstate error");
		exit(EXIT_FAILURE);
	}
	
	if (pthread_create(&thread_b, &thread_attr_b, black_thread, NULL) !=0 ) 
		{
			perror("THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
		}	
	 
	
	if (pthread_create(&thread_r, &thread_attr_r , read_thread, NULL) !=0 ) 
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
		
	

	sleep(5);
	CloseIPC_b();
	CloseIPC_r();

	printf("2 IPC were closed \n");
	return 0;
}



