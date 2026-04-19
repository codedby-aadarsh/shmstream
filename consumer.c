#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct{
    int number;
    char data[256];
}Frame;

int main(void){

    int fd = shm_open("/shm_buffer",O_RDONLY,0666);
    if(fd==-1){
        printf("error: cant open the shm buffer");
        goto exit;
    }

    sem_t *sem = sem_open("/shm_buffer",0);
    Frame *frame = mmap(NULL,sizeof(Frame),PROT_READ,MAP_SHARED,fd,0);
    sem_wait(sem); // blocks until the producer signals
    printf("Consumer read frame %d: %s\n", frame->number, frame->data);
    munmap(frame, sizeof(Frame));
    close(fd);
    return 0;
exit:
    return -1;
}