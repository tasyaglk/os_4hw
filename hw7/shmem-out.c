#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

const char *size = "posix-shar-size";
const char *shar_object = "posix-shar-object";

void sys_err(char *msg) {
    puts(msg);
    exit(1);
}

int main() {
    int shmid, shmid2;            // дескриптор разделяемой памяти
    int *sizze, *mas; // адрес числа в разделяемой памяти
    int num;
    int cnt;
    if ((shmid = shm_open(size, O_CREAT | O_RDWR, 0666)) == -1 ||
        (shmid2 = shm_open(shar_object, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("shm_open");
        sys_err("client: object is already open");
    } else {
        printf("Object is open: name = %s, id = 0x%x\n", shar_object, shmid);
    }

    //получить доступ к памяти
    mas = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    sizze = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid2, 0);
    if (size == MAP_FAILED || mas == MAP_FAILED) {
        printf("Error");
        return 1;
    }
    cnt = *sizze;
    printf("Еxpected number of numbers %d \n", cnt);
    for (int i = 0; i < cnt; i++) {
        num = mas[i];
        printf("server got random number - %d \n", num);
        sleep(1);
    }
  // это нужно разкоментить, если та штука в генерации все-таки работает(возможно у меня не на ли нуксе проблемы)
// int i = 0;
//   while(1){
  
//     num = mas[i];
//     if (num == 0) {
//         break;
//     }
//     i++;
//         printf("server got random number - %d \n", num);
//         sleep(1);
//   }


    // закрытие 
    if (close(shmid) == -1 || close(shmid2) == -1) {
        perror("close");
        exit(1);
    }

    // удаление разделяемой памяти
    if (shm_unlink(shar_object) == -1 || shm_unlink(size) == -1) {
        perror("shm_unlink");
        sys_err("server: error getting pointer to shared memory");
    }

    return 0;
}