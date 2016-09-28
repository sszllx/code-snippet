#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include <sys/time.h>
#include<unistd.h>

#define __USE_GNU
#include<sched.h>
#include<ctype.h>
#include<string.h>

int main(int argc, char* argv[])
{
    int num = sysconf(_SC_NPROCESSORS_CONF);
    int created_thread = 0;
    int myid;
    int i;
    int j = 0;

    cpu_set_t mask;
    cpu_set_t get;

    if (argc != 2)
        {
            printf("usage : ./cpu num\n");
            exit(1);
        }

    myid = atoi(argv[1]);/*绑定到的cpu*/

    printf("system has %i processor(s). \n", num);

    CPU_ZERO(&mask);
    CPU_SET(myid, &mask); /*掩码*/

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)/*0指当前进程*/
        {
            printf("warning: could not set CPU affinity, continuing...\n");
        }

    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1)
        {
            printf("warning: cound not get cpu affinity, continuing...\n");
        }
    for (i = 0; i < num; i++)
        {
            if (CPU_ISSET(i, &get))
                {
                    printf("this process %d is running processor : %d\n",getpid(), i);
                }
        }
    struct timeval tv;
    long long start_time,end_time;
    while(1)
        {
            gettimeofday(&tv,NULL);
            start_time = tv.tv_sec*1000000 + tv.tv_usec;
            end_time = start_time;
            while((end_time - start_time) < 60000)
                {
                    gettimeofday(&tv,NULL);
                    end_time = tv.tv_sec*1000000 + tv.tv_usec;
                }
            usleep(60000);
        }

    return 0;
}
