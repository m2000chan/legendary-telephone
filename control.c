#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

#define KEY_SM 125
#define KEY 124

int getVal(int sd){
  int value = semctl(sd, 0, GETVAL);
  return value;
}

void print_story(){
  int file = open("text.txt", O_RDONLY, 0644);
  char * story = malloc(2000);
  read(file, story, 2000);
  printf("Story: %s\n", story);
  close(file);
  free(story);
}

int main(int argc, char **argv){
  
  if( argc ){

    int sd;

    
    if( strcmp(argv[1], "-c") == 0){
      sd = semget(KEY, 1, IPC_EXCL | IPC_CREAT | 0644);
      
      if(sd == -1){
	printf("semaphore already exists\n");
      }

      else{
	printf("value set: %d\n", getVal(sd));
	semctl(sd, 0, SETVAL, 1);
        shmget(KEY_SM, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
	int file = open("text.txt", O_TRUNC | O_CREAT, 0644);
      }
    }
    
    else if(strcmp(argv[1], "-v") == 0){
      print_story();
    }
  
    else if(strcmp(argv[1], "-r") == 0){
      sd = semget(KEY, 1, 0644);
      int val = semctl(sd, 0, IPC_RMID);
      printf("semaphore removed: %d\n", val);
      int shmd = shmget(KEY_SM, sizeof(int), 0);
      val = shmctl(shmd, IPC_RMID, 0);
      printf("shared memory removed: %d\n", val);

      print_story();
      
      remove("text.txt");
    }
    
  }
}
