#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <csignal>
#include <iostream>

char shar_object[] = "memory";
int shmid;

pid_t pid;

void ExitProgram(int sigino)
{
  printf("\nПрограмма завершена");
  kill(pid, SIGINT);
  exit(0);
}

void sys_err (char *msg) {
  puts (msg);
  exit (1);
}

int main () {
  (void)signal(SIGINT, ExitProgram);
  pid_t own_pid = getpid();

  std::cout << "Мой pid: " << own_pid << "\n";
  
  printf("Введите pid клиента: ");

  std::cin >> pid;
  
  if ( (shmid = shm_open(shar_object, O_CREAT|O_RDWR, 0666)) == -1 ) {
    perror("shm_open");
    return -1;
  } else {
    printf("Object is open: name = %s, id = 0x%x\n", shar_object, shmid);
  }

  int* msg_p = (int*)mmap(0, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, shmid, 0);
  if (msg_p == (int*)-1 ) {
    perror("mmap");
  }

  while (1) {
    sleep(1);
    printf("%d\n", *msg_p);
  }

  return 0;
}

