#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
//#include<sys/ipc.h>
#include<semaphore.h>

#define N 5

sem_t mutex,customers,barbers;/*Three semaphors*/
int count=0;/*The number of customers waiting for haircuts*/

void* barber(void *arg);
void* customer(void *arg);

int main(int argc,char *argv[])
{
	pthread_t id1,id2,id3,id4;
	int status=0;

	/*Semaphore initialization*/
	sem_init(&mutex,0,1);
	sem_init(&customers,0,0);
	sem_init(&barbers,0,1);

	/*Barber_thread initialization*/
	status=pthread_create(&id1,NULL,barber,NULL);
	if(status!=0)
		perror("create barbers is failure!\n");

	/*Customer_thread initialization*/
	status=pthread_create(&id2,NULL,customer,NULL);
	if(status!=0)
		perror("create customers is failure!\n");
	status=pthread_create(&id3,NULL,customer,NULL);
	if(status!=0)
		perror("create customers is failure!\n");
	status=pthread_create(&id4,NULL,customer,NULL);
	if(status!=0)
		perror("create customers is failure!\n");

	/*Customer_thread first blocked*/
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	pthread_join(id4,NULL);
	pthread_join(id1,NULL);

	exit(0);
}

void* barber(void *arg)/*Barber Process*/
{
	int i=15;
	while(i--)
	{
		sem_wait(&customers);/*P(customers)*/
		sem_wait(&mutex);/*P(mutex)*/
		count--;
		printf("Barber:cut hair,count is:%d.\n",count);
		sem_post(&mutex);/*V(mutex)*/
		sem_post(&barbers);/*V(barbers)*/
		usleep(3);
	}
}

void* customer(void *arg)/*Customers Process*/
{
	int i=15;
	while(i--)
	{
		sem_wait(&mutex);/*P(mutex)*/
		if(count<N)
		{
			count++;
			printf("Customer:add count,count is:%d\n",count);
			sem_post(&mutex);/*V(mutex)*/
			sem_post(&customers);/*V(customers)*/
			sem_wait(&barbers);/*P(barbers)*/
		}
		else
			/*V(mutex)*/
			/*If the number is full of customers,just put the mutex lock let go*/
			sem_post(&mutex);
		usleep(1);
	}
}
