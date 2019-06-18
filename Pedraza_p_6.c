#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <fcntl.h>
#define SEM_NAME1 "/thomas1.mutex"
#define SEM_NAME2 "/thomas2.mutex"
#define SEM_NAME3 "/thomas3.mutex"

int main(int argc, char *argv[]) {
  //Declarating variables
  pid_t pid;
  int maxnum;
  sem_t *sem1;
  sem_t *sem2;
  sem_t *sem3;

  //atoi declaration
  int atoi(const char *str);

  //Semaphore declarations
  sem1 = sem_open(SEM_NAME1, O_CREAT, O_RDWR, 1);
  sem2 = sem_open(SEM_NAME2, O_CREAT, O_RDWR, 1);
  sem3 = sem_open(SEM_NAME3, O_CREAT, O_RDWR, 1);

  //Checks parameters
  if (argc != 2) {
    //Isn't only one parameter
    printf("Invalid use of command.\n");
    //unlinks all semaphores
    sem_unlink("/thomas1.mutex");
    sem_unlink("/thomas2.mutex");
    sem_unlink("/thomas3.mutex");
    return 1;
  }
  //Convert the parameter from string to int
  maxnum = atoi(argv[1]);

  //Have semaphore wait to create an offset of start time executions
  sem_wait(sem1);
  sem_wait(sem2);
  sem_wait(sem3);

  //First fork
  pid = fork();
  //If pid < 0 athere was a problem forking
  if (pid < 0) {
  fprintf(stderr, "Fork Failed\n");
  return 1;
  //First child
  } else if (pid == 0) {
    //1st child prints 0, 3, 6, ...
    for (int i = 0; i <= maxnum; i = i + 3) {
      //Has no sem_wait before the print because this is the child that executes first
      printf("1st Child: %d\n", i);
      sem_post(sem1);
      sem_wait(sem3);
    }
  //Parent process
  } else {
      // Fork the second child
      pid = fork();
      //If the pid < 0 then an error has occured
    if (pid < 0) {
      fprintf(stderr, "Second Fork Failed\n");
      return 1;
      //Second child
    } else if (pid == 0) {
      //Second child prints 2, 5, 8, ...
      for (int i = 2; i <= maxnum; i = i + 3) {
        //Prepare for critical section
        sem_wait(sem2);
        //Critical section
        printf("2nd child: %d\n", i);
        sem_post(sem3);
      }
      sem_post(sem3);
    //Parent process
    } else {
      //Parent prints 1, 4, 7, ...
      for (int i = 1; i <= maxnum; i = i + 3) {
        //Prepare for critical section
        sem_wait(sem1);
        //critical section
        printf("Parent:    %d\n", i);
        sem_post(sem2);
      }
      sem_post(sem2);

      //Unlink the semaphores
      sem_unlink("/thomas1.mutex");
      sem_unlink("/thomas2.mutex");
      sem_unlink("/thomas3.mutex");
    }
  }
  //End of main
  return 0;
}
