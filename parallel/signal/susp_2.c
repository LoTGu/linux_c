/* block.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void int_handler(int s)
{
   write(1, "!", 1);

}

int main()
{
    int i, j;
    sigset_t set;
    sigset_t oset;
    sigset_t saveset;

	signal(SIGINT, int_handler);  //注册在接收到SIGINT信号后调用int_handler函数
	sigemptyset(&set);
	sigaddset(&set, SIGINT);

    sigprocmask(SIG_UNBLOCK, &set, &saveset);
    sigprocmask(SIG_BLOCK, &set, &oset);

    for(j = 0; j < 1000; j++)
    {

        for(i = 0; i < 5; i++)
	     {
	          write(1, "*", 1);
		       sleep(1);
	     }
	     write(1, "\n", 1);
         sigsuspend(&oset);
/*
         sigset_t tmpset;
         sigprocmask(SIG_SETMASK, &oset, &tmpset);
         pause();
         sigprocmask(SIG_SETMASK, &tmpset, NULL);
*/
    }
    sigprocmask(SIG_SETMASK, &saveset, NULL);    //恢复进入man函数之前的信号状态

    exit(0);
}
