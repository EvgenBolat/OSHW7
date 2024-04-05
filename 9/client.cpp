#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>

pid_t pid;

int num;
int shmid;
char shar_object[] = "memory";

void sys_err(char *msg)
{
  puts(msg);
  exit(1);
}

void ExitProgram(int sigino)
{
  if (shm_unlink(shar_object) == -1)
  {
    printf("shm_unlink");
    exit(0);
  }
  printf("Выход");
  kill(pid, SIGINT);
  exit(0);
}

int main()
{
  (void)signal(SIGINT, ExitProgram);
  pid_t my_pid = getpid();

  std::cout << "Мой pid: " << my_pid << "\n";
  printf("Введите pid сервера: ");
  std::cin >> pid;

  srand(time(NULL));
  if ((shmid = shm_open(shar_object, O_CREAT | O_RDWR, 0666)) == -1)
  {
    printf("shm_open");
    return 0;
  }
  else
  {
    printf("Object is open: name = %s, id = 0x%x\n", shar_object, shmid);
  }

  if (ftruncate(shmid, sizeof(int)) == -1)
  {
    printf("ftruncate");
    return 0;
  }
  else
  {
    printf("Memory size set and = %lu\n", sizeof(int));
  }

  int *msg_p = (int *)mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
  if (msg_p == (int *)-1)
  {
    printf("mmap");
    return 0;
  }

  while (1)
  {
    num = random() % 1000;
    *msg_p = num;
    printf("random number: %d\n", num);
    sleep(1);
  }
  close(shmid);
  return 0;
}
