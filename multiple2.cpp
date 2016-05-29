#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include<unistd.h>

// this is a multiple con pro problem
using namespace std;

struct cake
{
    int id;
    char* type;
};

queue<cake> tray1;
queue<cake> tray2;
queue<cake> tray3;
sem_t empty1;
sem_t empty2;
sem_t empty3;
sem_t full1;
sem_t full2;
sem_t full3;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

void initialize()
{
    sem_init(&empty1, 0, 5);
    sem_init(&empty2, 0, 5);
    sem_init(&empty3, 0, 5);
    sem_init(&full1, 0, 0);
    sem_init(&full2, 0, 0);
    sem_init(&full3, 0, 0);
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_mutex_init(&mutex3, NULL);
}

void* produce(void* arg)
{
    char* type = (char*) arg;
    if(type == "X")
    {
        int i=0;
        while(true)
        {
            sem_wait(&empty1);
            usleep(2);
            cake temp= {i, "V"};

            pthread_mutex_lock(&mutex1);
            tray1.push(temp);
            printf("Producer produced Vanilla cake : %d\n",i);
            pthread_mutex_unlock(&mutex1);

            sem_post(&full1);
            i++;
            if (i==15) break;
        }
    }
    else if(type == "Y")
    {
        int i=0;
        while(true)
        {

            sem_wait(&empty1);
            usleep(2);
            cake temp= {i, "C"};

            pthread_mutex_lock(&mutex1);
            tray1.push(temp);
            printf("Producer produced Chocolate cake : %d\n",i);
            pthread_mutex_unlock(&mutex1);

            sem_post(&full1);
            i++;
            if (i==15) break;

        }
    }
}

void* decorate(void* arg)
{
    //for(int i=0; i<10; i++)
    int i=0;
    while(true)
    {
        sem_wait(&full1);
        usleep(2);

        pthread_mutex_lock(&mutex1);
        cake item = tray1.front();
        tray1.pop();
        printf("Decorator took %s cake : %d\n",item.type,item.id);
        pthread_mutex_unlock(&mutex1);

        if(item.type == "V")
        {
            sem_wait(&empty2);
            usleep(2);

            pthread_mutex_lock(&mutex2);
            tray2.push(item);
            printf("Decorator put Vanilla cake : %d\n",item.id);
            pthread_mutex_unlock(&mutex2);

            sem_post(&full2);
        }
        else
        {
            sem_wait(&empty3);
            usleep(2);

            pthread_mutex_lock(&mutex3);
            tray3.push(item);
            printf("Decorator put Chocolate cake : %d\n",item.id);
            pthread_mutex_unlock(&mutex3);

            sem_post(&full3);
        }

        sem_post(&empty1);
        i++;
        if (i==15) break;
    }
}

void* consume(void* arg)
{
    char* type = (char*)arg;
    if(type == "V")
    {
        int i=0;
        while(true)
        {
            sem_wait(&full2);
            usleep(2);

            pthread_mutex_lock(&mutex2);
            cake item = tray2.front();
            tray2.pop();
            printf("Consumer consumed %s cake : %d\n",item.type,item.id);
            pthread_mutex_unlock(&mutex2);

            sem_post(&empty2);
            i++;
            if (i==15) break;
        }
    }
    else if(type == "C")
    {
        int i=0;
        while(true)
        {

            sem_wait(&full3);
            usleep(2);

            pthread_mutex_lock(&mutex3);
            cake item = tray3.front();
            tray3.pop();
            printf("Consumer consumed %s cake : %d\n",item.type,item.id);
            pthread_mutex_unlock(&mutex3);

            sem_post(&empty3);
            i++;
            if (i==15) break;

        }
    }

}

int main()
{
    pthread_t producer1,producer2,producer3;
    pthread_t consumer1, consumer2;

    initialize();
    pthread_create(&producer1, NULL, produce, (void*)"X");
    pthread_create(&producer2, NULL, produce, (void*)"Y");
    pthread_create(&producer3, NULL, decorate, (void*)"Z");
    pthread_create(&consumer1, NULL, consume, (void*)"V");
    pthread_create(&consumer2, NULL, consume, (void*)"C");

    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(producer3, NULL);
    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    return 0;
}
