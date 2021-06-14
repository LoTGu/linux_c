
/* slowcat2.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>


#include "mytbf.h"

#define CPS  10
#define BUFF_SIZE 1024
#define BURST 100

int main(int argc, char **argv)
{
    int sfd, dfd = 1;
    int len = 0;
    int ret = 0;
    int pos = 0;
    char buff[BUFF_SIZE];
    mytbf_t *tbf;
    int size;
    
    if(argc < 2)
    {
         fprintf(stderr, "Usage: %s <src_file> <dst_file>\n", argv[0]);
         exit(1);
    }

    tbf = mytbf_init(CPS, BURST);
    if(tbf == NULL)
    {
	fprintf(stderr, "mytbf_init() failed!\n");
        exit(1);
    
    }

    do
	{
		sfd = open(argv[1], O_RDONLY);
		if(sfd < 0 )
		{
		    if(errno != EINTR)
			{
			    perror("fopen()");
			    exit(1);
			}
		}
    }while(sfd < 0);

    while(1)
    {
        size = mytbf_fetchtoken(tbf, BUFF_SIZE);
	if(size < 0)
	{
	    fprintf(stderr, "mytbf_fechtoken():%s\n", strerror(-size));
	    exit(1);
	}

    	while((len= read(sfd,buff, size)) < 0)
    	{
		    if(errno == EINTR)
			    continue;

    		perror("fopen()");
    		break;
    	}
    	if(len == 0)
    		break;
        if((size - len) > 0)    
            mytbf_returntoken(tbf, size - len);

    	pos = 0;
    	while(len > 0)
    	{
    		ret = write(dfd, buff, len);
    		if(ret < 0)
    		{
			    if(errno == EINTR)
			        continue;
    			perror("write()");
    			exit(1);
    		}
    		pos += ret;
    		len -= ret;
    	}
    }

    close(sfd);

    mytbf_destroy(tbf);

    exit(0);
}
