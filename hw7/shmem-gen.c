#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <signal.h>


const char *size = "posix-shar-size";
const char *shar_object = "posix-shar-object";

void sys_err(char *msg) {
    puts(msg);
    exit(1);
}

volatile sig_atomic_t server_running = 1;
void sigint_handler(int sig) {
    server_running = 0;
}


int main() {
    int shmid, shmid2; // дескриптор объекта памяти
    int *sizze, *mas;      // адрес количества чисел и сам массив чисел в разделяемой памяти
    int cnt;
    int num;
    printf("Сколько чисел необходимо сгенерировать?\n");
    scanf("%d", &cnt);
    if ((shmid = shm_open(size, O_CREAT | O_RDWR, 0666)) == -1 ||
        (shmid2 = shm_open(shar_object, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("shm_open");
        sys_err("server: object is already open");
    } else {
        printf("Object is open: name = %s, id = 0x%x\n", shar_object, shmid);
    }
    // Задание размера объекта памяти
    if (ftruncate(shmid, sizeof(int)) == -1 || ftruncate(shmid2, sizeof(int)) == -1) {
        perror("ftruncate");
        sys_err("server: memory sizing error");
        return 1;
    } else {
        printf("Memory size set and = %lu\n", sizeof(int));
    }

    //получить доступ к памяти
    mas = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    sizze = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid2, 0);
    if (sizze == (int *) -1 || mas == (int *) -1) {
        printf("Error");
        return 1;
    }
    srand(time(NULL));
    *sizze = cnt;
    for (int i = 0; i < cnt; i++) {
        num = random() % 1000;
        mas[i] = num;
        printf("client's random number - %d \n", num);
        sleep(1);
     }
  // тут если верить интернету должна обрываться работа, если на сервере оборвалась. Но у меня не сработало
  //  if (signal(SIGINT, sigint_handler) == SIG_ERR) {
  //       perror("signal");
  //       exit(EXIT_FAILURE);
  //   }

  // int i = 0;
  // while (server_running) {
  //       num = random() % 1000;
  //       mas[i] = num;
  //       printf("client's random number - %d \n", num);
  //       sleep(1);
  //   i++;
  //   }

    return 0;
}
