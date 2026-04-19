#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

typedef struct{
    int number;
    char data[256];
}Frame;

int main(void){

    int fd = shm_open("/shm_buffer",O_CREAT|O_RDWR|O_EXCL,0666);
    if(fd==-1){
        printf("error: failed to create or already exist");
        goto exit;
    }

    sem_t *sem = sem_open("/shm_buffer",O_CREAT,0666);
    
    ftruncate(fd,sizeof(Frame));
    Frame *frame = mmap(NULL,sizeof(frame),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    frame->number=1;
    strcpy(frame->data,"JPEG_FAKE_DATA");
    sem_post(sem); // signals the consumer data is ready 
    printf("Producer wrote frame %d\n", frame->number);    
    munmap(frame, sizeof(Frame));
    close(fd);

    return 0;

exit:
    return -1;
}