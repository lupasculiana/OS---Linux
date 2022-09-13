#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>

#define NR_THREADS 5
#define NR_THREADS_TREI 7
typedef struct
{
    int id;
    int proc;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
} TH_STRUCT;

int turn = 0;
int done = 0;
int trei = 0;

void *func_trei(void *arg)
{
TH_STRUCT *s = (TH_STRUCT*)arg;
    pthread_mutex_lock(s->lock);
    if(s->id ==5)
    {
    while(trei != 1)
    {
    pthread_cond_wait(s->cond, s->lock);
    
    }
    }
    info(BEGIN,4,s->id);
    info(END,4,s->id);
    pthread_mutex_unlock(s->lock);

    return NULL;
    
}
void *func(void *arg)
{
    TH_STRUCT *s = (TH_STRUCT*)arg;
    pthread_mutex_lock(s->lock);
   
    if(s->id == 4)
    {
        done = 1;
    }
    if(s->id == 3)
    {
        while(done != 1)
        {
            pthread_cond_wait(s->cond, s->lock);
        }
    }
     info(BEGIN,6,s->id);

    
    if(s->id == 4)
    {
        pthread_cond_broadcast(s->cond);
        pthread_cond_wait(s->cond, s->lock);
    }
    info(END,6,s->id);
    if(s->id ==2)
    {
    trei =1;
    pthread_cond_broadcast(s->cond);
    info(BEGIN,4,5);
    info(END,4,5);
    }
    
    
    turn++;
    if(s->id ==3)
    {
    pthread_cond_broadcast(s->cond);
    }
    if(turn == 4)
    {
        pthread_cond_broadcast(s->cond);
    }
   
    pthread_mutex_unlock(s->lock);

    return NULL;
}

int main()
{

    init();
    pid_t pid2=-1, pid3=-1, pid4=-1, pid5=-1, pid6=-1, pid7=-1;
    //root - P1
    info(BEGIN, 1, 0);
    pid2 = fork();
    waitpid(pid2,NULL,0);
    if(pid2 == 0)
    {
        //suntem pe p2
        //1st child of root - P2
        info(BEGIN, 2, 0);
        info(END, 2, 0);
        exit(0);
    }
    else
    {

        //inapoi pe parinte p1
        //2nd child of root - P3
        pid3 = fork();
        waitpid(pid3, NULL, 0);
        if(pid3 == 0)
        {
            info(BEGIN, 3, 0);
            info(END, 3, 0);
            exit(0);
        }
        else
        {
            // inapoi la parinte p1
            //3rd child of root - P4
            pid4 = fork();
            waitpid(pid4, NULL, 0);
            if(pid4 == 0)
            {
                info(BEGIN, 4, 0);
		//exercitiul 4 
		int j;
                        TH_STRUCT params_trei[NR_THREADS_TREI];
                        pthread_t tids_trei[NR_THREADS_TREI];
                        pthread_mutex_t lock_trei = PTHREAD_MUTEX_INITIALIZER;
                        pthread_cond_t cond_trei = PTHREAD_COND_INITIALIZER;

                        for(j=1; j<NR_THREADS_TREI; j++)
                        {
                        if(j!=5)
                        {
                        params_trei[j].proc = 4;
                            params_trei[j].id = j;
                            params_trei[j].lock = &lock_trei;
                            params_trei[j].cond = &cond_trei;
                            pthread_create(&tids_trei[j], NULL, func_trei, &params_trei[j]);
                        }
                        }
                        
                        for(j=1; j<NR_THREADS; j++)
                        {
                            pthread_join(tids_trei[j], NULL);
                        }
                       
                        pthread_cond_destroy(&cond_trei);
                        pthread_mutex_destroy(&lock_trei);
		//
                //suntem in P4
                //1st child of P4-P5
                pid5 = fork();
                waitpid(pid5,NULL,0);
                if(pid5 == 0)
                {
                    info(BEGIN, 5, 0);
                    //suntem in P5
                    //1st child of P5 - P6
                    pid6 = fork();
                    waitpid(pid6, NULL, 0);
                    if(pid6 == 0)
                    {
                        info(BEGIN, 6, 0);
                        //suntem in P6
                        //exercitiul 2
                        int i;
                        TH_STRUCT params[NR_THREADS];
                        pthread_t tids[NR_THREADS];
                        pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
                        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

                        for(i=1; i<NR_THREADS; i++)
                        {
                            params[i].id = i;
                            params[i].proc = 6;
                            params[i].lock = &lock;
                            params[i].cond = &cond;
                            pthread_create(&tids[i], NULL, func, &params[i]);

                        }
                        for(i=1; i<NR_THREADS; i++)
                        {
                        
                            pthread_join(tids[i], NULL);
                        }
                        pthread_cond_destroy(&cond);
                        pthread_mutex_destroy(&lock);

                        //
                        info(END, 6, 0);
                        exit(0);
                    }
                    info(END, 5, 0);
                    exit(0);
                }
                
                info(END, 4, 0);
                exit(0);
            }
            else
            {
                //inapoi la parinte p1
                //4th child of root - p7
                pid7 = fork();
                waitpid(pid7, NULL, 0);
                if(pid7 == 0)
                {
                    info(BEGIN,7,0);
                    info(END,7,0);
                    exit(0);
                }
            }
             wait(NULL);
              wait(NULL);
               wait(NULL);
        }
        info(END, 1, 0);

    }

    return 0;
}



