#define _GNU_SOURCE
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
#include <seccomp.h>
#include <grp.h>

int main()
{
	/* Launch the firlter seccomp in order to forbidden call systems non necessary to the programme */
	//seccomp();
	
	while (1) 
	{
	pthread_t thread_r; /* identify thread who will launch REAG PROG */
	pthread_t thread_b;	/* identify thread who will launch BLACK PROG */
	pthread_t thread_o;	/* identify thread who will launch ORCHESTRATOR PROG */
	pthread_attr_t thread_attr_b;
	pthread_attr_t thread_attr_r;
	
	/* Changement of group of Orchestrator to the UZI group  */
	gid_t uzi = gid_from_name("uzi");
	if ( setresgid(uzi,uzi,uzi) == -1 ) 
		{ 
			perror("setregid");
		}
	
	/* Creation of the 2 IPC READ and BLACK */
	CreateIPC_r();	
	CreateIPC_b() ; 
	
	printf("Launch IPC READ and IPC Black \n");
	printf("Launch 3 thread \n");
	
	/* When the thread bfor BLACK programme will be created, it will be detached in order to free all ressources at the exit */
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
	
		/* When the thread bfor READ programme will be created, it will be detached in order to free all ressources at the exit */
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
	
	/* Launch the thread for BLACK programme */
	if (pthread_create(&thread_b, &thread_attr_b, black_thread, NULL) !=0 ) 
		{
			perror("THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
		}	
	 
	/* Launch the thread for READ programme */
	if (pthread_create(&thread_r, &thread_attr_r , read_thread, NULL) !=0 ) 
		{
			perror("THREAD_ERROR_CREATE");
			return THREAD_ERROR ;	
		}
	
	/* Launch the thread for ORCHESTRATOR programme */
	if (pthread_create(&thread_o, NULL, orche_thread, NULL) !=0 ) 
		{
			perror("THREAD_ERROR_CREATE");
			return THREAD_ERROR;	
		}
	
	/* When the Orchestrator thread is finished, we can continue the programme */
	if ( ( pthread_join(thread_o, NULL) ) != 0 ) 
		{
			perror("THREAD_ERROR_JOIN");
			return THREAD_ERROR_JOIN ;	
		}
	
	/* Close the 2 IPC READ and BLACK */
	CloseIPC_b();
	CloseIPC_r();

	fprintf(stdout,"2 IPC were closed \n");
	fprintf(stdout,"END Programme Orchestrator before new launch in less than 30 minutes \n");
	sleep(5);
	}
	
	return 0;
}



