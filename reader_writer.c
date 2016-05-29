//Readers Writers Problem -OS Lab


#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>

sem_t mutex,writeblock;
int data = 0,rcount = 0;

void *reader(void *arg)
{

    int f=(int)arg;
    printf("Into reader %d ",f);
    sem_wait(&mutex);
    rcount = rcount + 1;
    if(rcount==1)
        sem_wait(&writeblock);
    sem_post(&mutex);
    printf("Data read by the reader %d is %d\n",f,data);
    usleep(1);
    sem_wait(&mutex);
    rcount = rcount - 1;
    if(rcount==0)
        sem_post(&writeblock);
    sem_post(&mutex);
}

void *writer(void *arg)
{
    int *f=((int*) arg);
    printf("Into writer %d",*f);
    sem_wait(&writeblock);
    data++;
    printf("Data writen by the writer%d is %d\n",*f,data);
    usleep(1);
    sem_post(&writeblock);
}

main()
{
    int i,b;
    int array[]= {0,1,2,3,4};
    pthread_t rtid[5],wtid[5];
    sem_init(&mutex,0,1);
    sem_init(&writeblock,0,1);
    for(i=0; i<=4; i++)
    {
        pthread_create(&wtid[i],NULL,writer,(void *)&array[i]);
        printf("Writer Created %d\n",i);
        pthread_create(&rtid[i],NULL,reader,(void *)&array[i]);
        printf("Reader Created %d\n",i);
    }
    for(i=0; i<=4; i++)
    {
        pthread_join(wtid[i],NULL);
        pthread_join(rtid[i],NULL);
    }
}
