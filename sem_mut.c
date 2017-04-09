#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define W_SIZE 1024
pthread_mutex_t w_mutex; 								/* protects both w_area and time_to_exit */
char w_area[W_SIZE];
int time_to_exit = 0;

void *thread_func() 
{
	sleep(1);
	pthread_mutex_lock(&w_mutex);
	while(strncmp("end", w_area, 3) != 0) 
	{
		printf("You input %d characters\n", strlen(w_area) -1);
		w_area[0] = '\0';
		pthread_mutex_unlock(&w_mutex);
		sleep(1);
		pthread_mutex_lock(&w_mutex);
		while (w_area[0] == '\0' ) 
		{
			pthread_mutex_unlock(&w_mutex);
			sleep(1);
			pthread_mutex_lock(&w_mutex);
		}
	}	
	time_to_exit = 1;
	w_area[0] = '\0';
	pthread_mutex_unlock(&w_mutex);
	pthread_exit(0);
}

int main() 
{
	int res;
	pthread_t a_thread;
	void *thread_result;
	res = pthread_mutex_init(&w_mutex, NULL);
	if (res != 0) 
	{
		perror("Mutex Initialization failed\n");
		exit(EXIT_FAILURE);
	}
	res = pthread_create(&a_thread, NULL, thread_func, NULL);
	if (res != 0) 
	{
		perror("thread creation failed\n");
		exit(EXIT_FAILURE);
	}
	pthread_mutex_lock(&w_mutex);
	printf("Input some text. Enter ‘end’ to finish\n");
	while(!time_to_exit) 
	{
		fgets(w_area, W_SIZE, stdin);
		pthread_mutex_unlock(&w_mutex);
		while(1) 
		{
			pthread_mutex_lock(&w_mutex);
			if (w_area[0] != '\0') 
			{
				pthread_mutex_unlock(&w_mutex);
				sleep(1);
			}
			else 
			{	
				break;
			}	
		}
	}
	pthread_mutex_unlock(&w_mutex);
	printf("\n Waiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if (res != 0) 
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}
	printf("Thread joined\n");
	pthread_mutex_destroy(&w_mutex);
	exit(EXIT_SUCCESS);
	return 0;
}


