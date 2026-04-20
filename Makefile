CC = gcc
CFLAGS = -Wall -g
all : producer consumer
producer: producer.c
	$(CC) $(CFLAGS) -o producer producer.c

consumer: consumer.c 
	$(CC) $(CFLAGS) -o consumer consumer.c

clean:
	rm -f producer
	rm -f /dev/shm/shm_buffer
	rm -f /dev/shm/sem.*
	rm -f consumer
