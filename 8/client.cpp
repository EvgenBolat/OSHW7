#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

void sys_err(char *msg)
{
  puts(msg);
  exit(1);
}

int main()
{
  int shmid;
  char shar_object[] = "memory";
  int num;

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

  int amounts = 0;
  while (amounts < 100)
  {
    num = random() % 1000;
    *msg_p = num;
    printf("random number: %d\n", num);
    sleep(1);
    amounts++;
  }
  *msg_p = -1;

  close(shmid);
  return 0;
}
