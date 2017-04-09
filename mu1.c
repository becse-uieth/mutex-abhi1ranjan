#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
 
int myglobal=0;
pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;
 
void *thread_function(void *arg) {
  int i,j;
  for ( i=0; i<20; i++ ) {
    pthread_mutex_lock(&mymutex);
    j=myglobal;
    j=j+1;
    printf(".");
    fflush(stdout);
    sleep(1);
    myglobal=j;
    pthread_mutex_unlock(&mymutex);
  }
  return NULL;
}
 
int main() {
 
    pthread_t mythread;
    int i,r;
 
    r = pthread_create( &mythread, NULL, thread_function, NULL);
    if( r !=0)	  {
    	perror("error creating thread.\n");
    	exit(EXIT_FAILURE);
    }
 
    for ( i=0; i<20; i++) {
    	pthread_mutex_lock(&mymutex);
    	myglobal=myglobal+1;
    	pthread_mutex_unlock(&mymutex);
    	printf("o");
    	fflush(stdout);
    	sleep(1);
    }
 
    r = pthread_join ( mythread, NULL );  
    if( r != 0)	{
    	perror("error joining thread.\n");
    	exit(EXIT_FAILURE);
    }
 
    printf("\nmyglobal equals %d\n",myglobal);
 
    exit(0);
    return 0;
}

