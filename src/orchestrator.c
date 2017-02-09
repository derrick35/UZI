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


int main()
{
	while (1) 
	
	{
	pthread_t thread_r; /* identify thread who will launch REAG PROG */
	pthread_t thread_b;	/* identify thread who will launch BLACK PROG */
	pthread_t thread_o;	/* identify thread who will launch ORCHESTRATOR PROG */
	pthread_attr_t thread_attr_b;
	pthread_attr_t thread_attr_r;
	
	
	/* Init the filter SECCOMP  */
	scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL); // default action: kill
	 if (!ctx) 
		{
			fprintf(stderr, "Failed to initialize libseccomp\n");
			return -1;
		}

	/* setup basic whitelist	*/
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(stat), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(munmap), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigaction), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigprocmask), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(access), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(nanosleep), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(clone), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgget), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgrcv), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgctl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgsnd), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getrlimit), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(arch_prctl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(prctl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(futex), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(set_tid_address), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(set_robust_list), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(wait4), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigreturn), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(madvise), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup2), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getpid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fcntl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(geteuid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getppid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(restart_syscall), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(set_robust_list), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(pipe2), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(seccomp), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getcwd), 0);

	
	
	/* build and load the filter	*/
	 if (seccomp_load(ctx) != 0) 
		{
			fprintf(stderr, "Failed to load the filter in the kernel\n");
			return -1;
		}
  
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

	fprintf(stdout,"2 IPC were closed \n");
	fprintf(stdout,"END Programme Orchestrator before new launch in less than 30 minutes \n");
	sleep(40);	
	}
	
	return 0;
}



