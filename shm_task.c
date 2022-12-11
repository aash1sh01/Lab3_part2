#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void Poor_Student_Process(int []);
void Dear_Old_Dad_Process(int []);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
    int    status;

      
     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error ***\n");
          exit(1);
     }
     
     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error ***\n");
          exit(1);
     }

     ShmPTR[0] = 100; //  BankAccount
     ShmPTR[1] = 0;    //  Turn
     printf("Original Account: %d\n", ShmPTR[0]);

     pid = fork();
     if (pid < 0) {
          printf("*** fork error ***\n");
          exit(1);
     }
     else if (pid == 0) {
          Poor_Student_Process(ShmPTR);
          exit(0);
     }
     else{
       Dear_Old_Dad_Process(ShmPTR);
     }
  wait(&status);
  printf("Process has detected the completion of its child...\n");
  shmdt((void *) ShmPTR);
  printf("Process has detached its shared memory...\n");
  shmctl(ShmID, IPC_RMID, NULL);
  printf("Process has removed its shared memory...\n");
  printf("Process exits...\n");
}

void  Poor_Student_Process(int  SharedMem[])
{
  while(1){
    sleep(rand()%5);
    int account = SharedMem[0];
    while(SharedMem[1]!=1);
    int balance = rand()%50;
    printf("Poor Student needs $%d\n", balance);
    if (balance<=account){
      account -= balance;
      printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
    }
    else{
      printf("Poor Student: Not Enough Cash ($%d)\n", account );
    }
    SharedMem[0] = account;
    SharedMem[1] = 0;
  }
}

void Dear_Old_Dad_Process(int SharedMem[])
{

  while(1){
    sleep(rand()%5);
    int account = SharedMem[0];
    while(SharedMem[1]!=0);
    if (account<=100){
      int balance = rand()%100; // 0-100
      if (balance  % 2 ==0){
        account += balance;
        printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
      }
      else{
        printf("Dear old Dad: Doesn't have any money to give\n");
      }
    }
    else{
      printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
    }
    
    SharedMem[0] = account;
    SharedMem[1] = 1;
  }
}