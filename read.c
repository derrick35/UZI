#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <sys/stat.h>

int main( void )
  {
     /*struct stat info;
    struct group* g;
   

    if( ( g = getgrgid( info.st_gid) ) == NULL ) {
      fprintf( stderr, "getgrgid: NULL pointer\n" );
      return( EXIT_FAILURE );
    }
    printf( "group name:%s\n", g->gr_name ); */
    
    size_t len;
    char *buffer = malloc(len);
    struct group result;
	struct group *resultp;
    int e = getgrnam_r("uzi", &result, buffer, len, &resultp);
    printf("%d \n",e);
    
    
    return( EXIT_SUCCESS );
  }
