#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>

typedef struct{
    int number;
    char data[256];
}Frame;

void sig_handler(){
    shm_unlink("/shm_buffer");
    sem_unlink("/shm_empty");
    sem_unlink("/shm_data");
}

int main(void){

    int fd = shm_open("/shm_buffer",O_RDONLY,0666);
    if(fd==-1){
        printf("error: cant open the shm buffer");
    }
    sem_t *empty_sem = sem_open("/shm_empty",0);
    sem_t *sem = sem_open("/shm_data",0);

    Frame *frame = mmap(NULL,sizeof(Frame),PROT_READ,MAP_SHARED,fd,0);
    close(fd);
    signal(SIGINT,sig_handler);
    
    int frame_num =0;
    sem_post(empty_sem);
    while(frame_num<100){
    sem_wait(sem); // blocks until the producer signals
    printf("Consumer read frame %d: %s\n", frame->number, frame->data);
    frame_num = frame->number;

    sem_post(empty_sem);
    }
    munmap(frame, sizeof(Frame));
    shm_unlink("/shm_buffer");
    sem_unlink("/shm_empty");
    sem_unlink("/shm_data");
    return 0;
}
