#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <csignal>

char shar_object[] = "memory";
int shmid;

void sys_err(char *msg)
{
  puts(msg);
  exit(1);
}

int main()
{
  if ((shmid = shm_open(shar_object, O_CREAT | O_RDWR, 0666)) == -1)
  {
    printf("shm_open");
    return 0;
  }
  else
  {
    printf("Object is open: name = %s, id = 0x%x\n", shar_object, shmid);
  }

  int *msg_p = (int *)mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
  if (msg_p == (int *)-1)
  {
    printf("mmap");
    return 0;
  }

  while (1)
  {
    sleep(1);
    if (*msg_p == -1)
    {
      if (shm_unlink(shar_object) == -1)
      {
        printf("shm_unlink");
        return 0;
      }
    }
    printf("%d\n", *msg_p);
  }

  if (shm_unlink(shar_object) == -1)
  {
    printf("shm_unlink");
    return 0;
  }

  return 0;
}
