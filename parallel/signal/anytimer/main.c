#include <stdio.h>
#include <stdlib.h>

static void f2(void *p)
{
    printf("f2():%s\n", p);
}

static void f1(void *p)
{
    printf("f1():%s\n", p);
}

int main(int argc, char const *argv[])
{
    int job1, job2, job3;

    puts("Begin!\n");

    at_addjob(5, f1, "aaa");
    if(job1 < 0)
    {

    	fprintf(stderr, "at_addjob():%s\n", strerror(-job1));
    	exit(1);
    }
    at_addjob(2, f2, "bbb");
    at_addjob(7, f1, "ccc");

    puts("End!\n");

    while(1)
    {

    	write(1, ".", 1);
    	sleep(1);
    }	

	exit(0);
}








