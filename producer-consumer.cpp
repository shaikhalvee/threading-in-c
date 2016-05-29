#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <unistd.h>
using namespace std;

queue<int> tray;
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void initialize()
{
    sem_init(&empty, 0, 5);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
}


void* produce(void* arg)
{
    int count = (int) arg;
    for(int i = 1; i<= count; i++)
    {
        sem_wait(&empty);
        usleep(2);

        pthread_mutex_lock(&mutex);
        tray.push(i);
        printf("Producer produced item : %d\n",i);
        pthread_mutex_unlock(&mutex);

        sem_post(&full);
    }
}

void* consume(void* arg)
{
    int count = (int) arg;
    for(int i = 1; i<= count; i++)
    {
        sem_wait(&full);
        usleep(2);

        pthread_mutex_lock(&mutex);
        int item = tray.front();
        tray.pop();
        printf("Consumer consumed item : %d\n",item);
        pthread_mutex_unlock(&mutex);

        sem_post(&empty);
    }
}

int main()
{
    pthread_t producer;
    pthread_t consumer;
    int amount = 10;

    initialize();
    pthread_create(&producer, NULL, produce, (void*)amount);
    pthread_create(&consumer, NULL, consume, (void*)amount);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    return 0;
}
