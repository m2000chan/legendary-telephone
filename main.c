#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sem.h>
#include <fcntl.h>
#define semkey 124
#define smkey 125

void semcon(int semid){
  struct sembuf s;
  s.sem_num = 0;
  s.sem_op = -1;
  s.sem_flg = 0;
  
  semop(semid, &s, 1);
}

void semdcon(int semid){
  struct sembuf s;
  s.sem_num = 0;
  s.sem_op = 1;
  s.sem_flg = 0;
  
  semop(semid, &s, 1);
}

int main(){
  int semid = semget(semkey, 1, 0644);
  
  semcon(semid); //connects to semaphore
  
  int shmid = shmget(smkey, sizeof(int), 0644); 
  int *p = shmat(shmid, 0, 0); //attaches a variable to shared memory
  int fd = open("text.txt", O_RDWR,  0644); //opens file

  lseek(fd, -(*p), SEEK_END); //length of last line stored in shared memory is used to move the file pointer to the correct position, by moving backwards the length from the end of the file
  char *last = (char *) calloc(* p + 1, sizeof(char)); //creates a pointer
  read(fd, last, *p); //get last line
  
  printf("Most recent line: %s\n", last);
  printf("Next line: ");
  char in[100];
  fgets(in, sizeof(in), stdin);
  write(fd, in, strlen(in));

  *p = strlen(in); //updates value
  
  close(fd); //closes file
  shmdt(p); //detaches a variable from shared memory
  semdcon(semid); //disconnects from semaphore
  
  return 0;
}
