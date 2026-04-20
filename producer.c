#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>

typedef struct{
    int number;
    char data[256];
}Frame;

void sig_handler(){
    shm_unlink("/shm_buffer");
    sem_unlink("/shm_data");
    sem_unlink("/shm_empty");
}
int main(void){

    int fd = shm_open("/shm_buffer",O_CREAT|O_RDWR|O_EXCL,0666);
    if(fd==-1){
        printf("error: failed to create or already exist");
    }
    signal(SIGINT,sig_handler);
    sem_t *empty_sem = sem_open("/shm_empty",O_CREAT,0666,1);
    sem_t *sem = sem_open("/shm_data",O_CREAT,0666,0);
    Frame *frame = mmap(NULL,sizeof(frame),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ftruncate(fd,sizeof(Frame));
    close(fd);
    int frame_num=0;
    printf("about to enter the loop");
    while(frame_num<100){
    sem_wait(empty_sem);
    frame->number=++frame_num;
    snprintf(frame->data,256,"jpeg data wrote : %d",frame->number);
    sem_post(sem); // signals the consumer data is ready 
    printf("Producer wrote frame %d\n", frame->number);    
    usleep(1000);
    }
    munmap(frame, sizeof(Frame));
    shm_unlink("/shm_buffer");
    sem_unlink("/shm_empty");
    sem_unlink("/shm_data");
    return 0;
}
