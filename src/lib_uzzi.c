#define _GNU_SOURCE
#include "../include/lib_uzzi.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <error.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <seccomp.h>


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
ipc_code CloseIPC_r() 
{
	if ( msgctl(msg_id_r, IPC_RMID, NULL) == -1)
		{
			if (errno == EPERM ) 
				{
					perror(" Close IPC_READ");
					return IPC_ERROR;
				}
			else
				{
					perror("Impossible to close IPC_READ");
					return IPC_ERROR;
				}
		}
		
	return msgctl(msg_id_r, IPC_RMID, NULL);
}

ipc_code CloseIPC_b() 
{
	if ( msgctl(msg_id_b, IPC_RMID, NULL) == -1)
		{
			if (errno == EPERM ) 
				{
					perror("Close IPC_BLACK");
					return IPC_ERROR;
				}
			else
				{
					perror("Impossible to close IPC_BLACK");
					return IPC_ERROR;
				}
		}
		
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
			perror("IPC_ERROR_R") ; 
			return IPC_ERROR_R ; 
		}
	if ( *msg == NULL) 
		{ 
			perror("IPC_ERROR_R") ; 
			return IPC_ERROR_R ;   
		}
	if ( (len = msgrcv(msg_id, (void *) &ipcMsg, sizeof(squidLog) +1, 0, 0) ) == -1) 
		{
			return IPC_ERROR_R ;
		}
	if ((*msg = (squidLog * ) calloc (1,(size_t) len ) ) == NULL) 
		{
			perror("calloc_error");
			return IPC_ERROR_R ; 
		}
	
	(void)memmove( *msg, &ipcMsg, (size_t) len ) ;
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
	if (  (ipcMsg =  (squidLog * ) calloc(MAX ,sizeof(squidLog)) ) == NULL )
		{
			perror("IPC_ERROR_W") ; 
			return IPC_ERROR_W;
		}
	if (  msg == NULL )
		{
			perror("IPC_ERROR_W") ; 
			return IPC_ERROR_W;
		}
	
	
	ipcMsg->mtype = msg->mtype ;
	ipcMsg->time = msg->time ;
	strlcpy(ipcMsg->clientIpAdress,msg->clientIpAdress,LOW_SIZE);
	strlcpy(ipcMsg->urlDest,msg->urlDest, MAX_SIZE);
	strlcpy(ipcMsg->user,msg->user,LOW_SIZE);
	
	if ( (msgsnd(msg_id, ipcMsg, sizeof(squidLog) +1 ,0)) == -1 )
		{
			perror("IPC_ERROR_W") ; 
			return IPC_ERROR_W;
		} 
	free(ipcMsg);
	return IPC_SUCCESS;
}

/* ********************************************************************* */
/* ***************  Function from BSD environment ************** */
/* ********************************************************************* */
size_t strlcpy(char *dst, const char *src, size_t dsize)
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
	
	return (size_t) (src - osrc - 1);	/* count does not include NUL */
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
		/*POUR LE DEBUG
		char *lu;
		if ( (lu = (char *)calloc(MAX, sizeof(char)) ) == NULL )
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	
	while ( fgets(lu, MAX, fr) != NULL ) 
		{
			fputs(lu, stdout); 
		}	*/
		
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
	squidLog ipcMsg_r ;
	squidLog *ipcMsg_b ;
	uid_t euid = geteuid();
	
	 if ( (ipcMsg_b = (squidLog*)calloc( 1 , sizeof(squidLog)) ) == NULL )
			{
				perror("THREAD_ERROR_ORCHE_CALLOC");
				exit(EXIT_FAILURE);
			}  
	
	 if ( (msg = (squidLog*)calloc(1 , sizeof(squidLog) ) ) == NULL )
			{
				perror("THREAD_ERROR_ORCHE_CALLOC");
				exit(EXIT_FAILURE);
			}  	
	
	 printf("Begin Orchestrator \n");
	 printf(" Avant chgt uid = %d , euid = %d et gid = %d \n",getuid(), geteuid(), getgid());
	/* Ochestrator will have no privilege and belong to uzi group */ 
	
	uid_t nobody = uid_from_name("nobody");
	if ( setresuid(euid,nobody,nobody) == -1 )
		{
			perror("setreuid");
			exit(EXIT_FAILURE);
		}
	
	printf(" Apres chgt  uid = %d et euid = %d gid = %d \n",getuid(), geteuid(), getgid());
		
	 do
		{
			free(msg);
			if ( ReadIPC(&msg, ipcMsg_r, msg_id_r) == IPC_ERROR_R )
				{
					CloseIPC_r();
				}
			printf("Reception sur orchestrator : %s / %s / %s \n",msg->clientIpAdress, msg->urlDest, msg->user); 
			if ( WriteIPC(msg, ipcMsg_b, msg_id_b) == IPC_ERROR_W ) 
				{
					CloseIPC_b();
				}
														
		} while (msg->mtype != 6); // no message to read if mtype = 6
	
	free(msg);
	free(ipcMsg_b);
	
	/* Orchestrator will have again root privilege */
	if (setresuid(0,0,nobody) == -1) 
		{
			perror("setreuid");
		}
		
	printf(" Apres modif  uid = %d et euid = %d \n",getuid(), geteuid());
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
	
	free(lu);
	pthread_exit(NULL);
}

/* ************************************************** */
/* ******************* Changement of identity ******* */
/* ************************************************** */

int change_ids(uid_t uid, gid_t gid)
{

	/* Changement of gid */
    if (setresgid(gid, gid, gid) != 0)
		{
			perror("Setresgid");
			exit(EXIT_FAILURE);
		}
    // Disengage potential additional groups 
    if (setgroups(0, NULL) != 0)
		{
			perror("Setgroups");
			exit(EXIT_FAILURE);
		}
 
    // Changement of uid in order not to be root.
    // We change uid, euid and suid of process 
    if (setreuid(uid, uid) != 0)
		{
			perror("Setreuid");
			exit(EXIT_FAILURE);
		}
	return 0;
}


/* ********************************************************************** */
/* ******************* Get back the uid from the name of a group  ******* */
/* ********************************************************************** */
uid_t uid_from_name(const char* name){
   struct passwd* pw;
   pw = getpwnam(name);
   if (pw == NULL) 
	{
		perror("getpwnam failed with");
		return 2;
	}
   return pw->pw_uid;
}	


/* ********************************************************************** */
/* ******************* Get back the gid from the name of a group  ******* */
/* ********************************************************************** */
gid_t gid_from_name(const char* group){  
   struct group* gr;
   gr = getgrnam(group);
   if (gr == NULL) 
	{
		perror("getgrnam failed with");
		return 2;
	}
   return gr->gr_gid;
}


/* ******************************************************************************************** */
/* ***************  Open a file which name is written in the configuration file  ************** */
/* ******************************************************************************************** */
FILE* openDir(FILE *file_config)
{
	FILE *file_dest;
	char *line = NULL ;
	line = (char *)calloc(MAX_SIZE , sizeof(char));	
	char *pointeur = NULL;
	pointeur = (char *)calloc(MAX_SIZE , sizeof(char));	
	char *path = NULL;
	char *com = "#";
	path = (char *)calloc(MAX_SIZE , sizeof(char));	
	
	/* Read the file_config line by line and search the path to the blacklist.txt file */
	while ( fgets(line, MAX_SIZE , file_config) != NULL ) 
	{
		if ( strstr(line, com) == NULL ) // dont take into account commentaries
			{
				if ( strtok_r(line,"=\n",&pointeur) == NULL) 
					{
						exit(EXIT_FAILURE);
					}
				path = strtok_r(NULL,"=\n",&pointeur);
				file_dest = fopen(path, "r");
				if (file_dest == NULL)
					{
						if (errno == EINTR)
							{
								do 
									{
										file_dest = fopen(path, "r"); 
									}		
								while (errno == EINTR);
							}
						else 
							{
								perror("openDir");
								exit(EXIT_FAILURE);
							}
					}	
			}
	}
		fclose(file_config);
		return file_dest;
}

/* ***************************************************************************** */
/* ***************  Search forbidden site from the blacklist fil  ************** */
/* ***************************************************************************** */
int search_forbidden_site(FILE *file, squidLog *msg)
{
	char *p_search = NULL; 	
	char *ligne ;	
	char *buffer;
	FILE *file_config = fopen("black.config", "r");
	if (file_config == NULL)
		{
			if (errno == EINTR)
				{
					do {file_config = fopen("black.config", "r"); }
					while (errno == EINTR);
				}
			else 
				{
					perror("open config");
					exit(EXIT_FAILURE);
				}
		}
	
	file = openDir(file_config);
	
	if ( (buffer = (char *)calloc((MAX_SIZE +1 ) , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(buffer);
			exit(EXIT_FAILURE);
		}
	 
	 if ( (p_search = (char *)calloc((MAX_SIZE +1) , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(p_search);
			exit(EXIT_FAILURE);
		}
	if ( (ligne = (char *)calloc(1024 , sizeof(char))) == NULL)
		{
			perror("calloc");
			free(ligne);
			exit(EXIT_FAILURE);
		}
	if ( file != NULL)
		{
			/* Read the file line by line and search the message msg->urlDest  */
			while( fgets(ligne, MAX_SIZE , file) != NULL )
				{
					if ( ( p_search = strstr(ligne, msg->urlDest) ) != NULL)
						{
							snprintf(buffer, MAX_SIZE +1 ,"Forbidden url : %s \t from %s \t with IP adress %s \n", msg->urlDest, msg->user,msg->clientIpAdress);
							fprintf(stdout,"%s \n", buffer);
						}
				}
		}
		
	free(p_search);
	free(ligne);
	free(buffer);
	fclose(file);
	return 0; 
}

/* ***************************************************************************************************************************** */
/* ***************  Launch the firlter seccomp in order to forbidden call systems non necessary to the programme  ************** */
/* ***************************************************************************************************************************** */
int seccomp()
{
	/*  Init the filter SECCOMP  */
	scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL); // default action: kill
	 if (!ctx) 
		{
			perror( "Failed to initialize libseccomp\n");
			exit(EXIT_FAILURE);
		}

	/* setup basic whitelist	*/
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(access), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(arch_prctl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(clone), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(connect), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup2), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fcntl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(futex), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getcwd), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(geteuid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getgid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getpid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getppid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getresuid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getrlimit), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getuid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(lseek), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(madvise), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgctl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgget), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgrcv), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(msgsnd), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(munmap), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(nanosleep), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(pipe2), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(prctl), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(restart_syscall), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigaction), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigprocmask), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigreturn), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(setgroups), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(setresgid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(setregid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(setreuid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(setresuid), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(set_robust_list), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(set_tid_address), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(stat), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(socket), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(tgkill), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(wait4), 0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);

		
	/* build and load the filter	*/
	 if (seccomp_load(ctx) != 0) 
		{
			fprintf(stderr, "Failed to load the filter in the kernel\n");
			return -1;
		}
	return 0;
}
