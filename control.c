#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sem.h>
#include <fcntl.h>

#define KEY_SM 125

int getVal(int sd){
  int value = semctl(sd, 0, GETVAL);
  return value;
}

void print_story(){

  int file = open("text.txt", O_RDONLY, 644);
  char * story = malloc(2000);
  read(file, story, 2000);
  printf("Story: %s\n", story);
  close(file);
  
}

int main(int argc, char **argv){
  
  if( argc ){

    int sd;
    int KEY = 124;

    int file = open("text.txt", O_CREAT, 644);
    
    if( strcmp(argv[1], "-c") == 0){
      sd = semget(KEY, 1, IPC_EXCL | IPC_CREAT | 0600);
      
      if(sd == -1){
	printf("semaphore already exists\n");
      }

      else{
	printf("value set: %d\n", getVal(sd));
	semctl(sd, 0, SETVAL, 1);
	shmget(KEY_SM, 2000, IPC_CREAT);
      }
    }
    else if(strcmp(argv[1], "-v") == 0){
      sd = semget(KEY, 1, 0600);
      printf("semaphore value: %d\n", getVal(sd));
      print_story();
    }
  
    else if(strcmp(argv[1], "-r") == 0){
      sd = semget(KEY, 1, 0600);
      int val = semctl(sd, 0, IPC_RMID);
      printf("semaphore removed: %d\n", val);
      print_story();
    }
  }
}
