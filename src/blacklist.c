#include "../include/lib_uzzi.h"
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
#include <sys/mman.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>

int main () 
{
	squidLog ipcMsg_b;
	squidLog *msg  ;
	
	if ( (msg = (squidLog*)calloc(1, sizeof(squidLog) ) ) == NULL )
		{
			perror("ERROR_BLACK_CALLOC");
			exit(EXIT_FAILURE);
		} 
	
	/* Blacklist will have no privilege and belong to uzi group  */	 
	gid_t uzi = gid_from_name("uzi");
	uid_t nobody = uid_from_name("nobody");
	change_ids(nobody, uzi);

	FILE *file = NULL ;
	doOpenIPC_b(0); // Open the IPC BLACK
	
	/* We search forbidden sites from the blacklist file */
	do
		{
			if ( ReadIPC(&msg, ipcMsg_b, msg_id_b) == IPC_ERROR_R ) 
				{
					CloseIPC_b();
				}
			
			search_forbidden_site(file, msg);
			
				 
		}  while (msg->mtype != 6);
		
	free(msg);
		
	return 0;
	
}		
