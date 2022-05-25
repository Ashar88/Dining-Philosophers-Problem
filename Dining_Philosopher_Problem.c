#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int num_Philosopher;
int num_chopsticks;
pthread_t *philosopher;
pthread_mutex_t *chopstick;

void dine(int n);

int main()
{
// int n;

  printf("Enter the numbers of Philosophers and corresponding Chopsticks: ");
  scanf("%d",&num_Philosopher);

  num_chopsticks= num_Philosopher;
  philosopher= (pthread_t *) malloc(num_Philosopher* sizeof(pthread_t));
  chopstick= (pthread_mutex_t *) malloc(num_Philosopher* sizeof(pthread_mutex_t));


  int status;
  void *msg;

  // Initialise the semaphore array
  for (int i = 0; i < num_chopsticks; i++)
  {
    status = pthread_mutex_init(&chopstick[i], NULL);
    if (status == -1) // Check if the mutex is initialised successfully
    {
      printf("\n Mutex initialization failed");
      exit(1);
    }
  }

  // Run the philosopher Threads using *dine() function
  for (int i = 0; i < num_Philosopher; i++)
  {
    status = pthread_create(&philosopher[i], NULL, (void *)dine, (int *)i);
    if (status != 0){
      printf("\n Thread creation error \n");
      exit(1);
    }
  }

  // Wait for all philosophers threads to complete executing (finish dining) before closing the program
  for (int i = 0; i < num_Philosopher; i++)
  {
    status = pthread_join(philosopher[i], &msg);
    if (status != 0) {
      printf("\n Thread join failed \n");
      exit(1);
    }
  }

  // Destroy the chopstick Mutex array
  for (int i = 0; i <num_chopsticks; i++)
  {
    status = pthread_mutex_destroy(&chopstick[i]);
    if (status != 0)   {
      printf("\n Mutex Destroyed \n");
      exit(1);
    }
  }
  return 0;
}

void dine(int n)
{
  long int s=0;
    while(1) {
       
        // printf("\nPhilosopher % d is thinking ", n);
       s= syscall(332,n,0,4);
       printf("\nSystem Call called with return value: ",s);
        sleep(5);

      if(n==num_Philosopher-1)  //resolving deadlock if all philosopher took the left chopstick and waiting to take the right one.
      {
            pthread_mutex_lock(&chopstick[(n + 1) % num_chopsticks]);  //picking the right chopstick.
            pthread_mutex_lock(&chopstick[n]); //picking the left chopstick.
      }
      else{
            pthread_mutex_lock(&chopstick[n]); //picking the left chopstick.
          pthread_mutex_lock(&chopstick[(n + 1) % num_chopsticks]);  //picking the right chopstick.
      }
        
        int right=(n + 1) % num_chopsticks;
        // printf("\nPhilosopher %d takes %dth and %dth  chopsticks.\n", n,n,right);
        s=syscall(332,n,right,0);
       printf("\nSystem Call called with return value: ",s);

// After picking up both the chopstick philosopher starts eating
        
        // printf("\nPhilosopher % d is eating ", n);
          s=syscall(332,n,0,1);
       printf("\nSystem Call called with return value: ",s);
         sleep(3);

        pthread_mutex_unlock(&chopstick[n]);    //left
        pthread_mutex_unlock(&chopstick[(n + 1) % num_chopsticks]);   //right

        // printf("\nPhilosopher % d Finished eating ", n);
        s=syscall(332,n,right,3);
       printf("\nSystem Call called with return value: ",s);
     }
} 



