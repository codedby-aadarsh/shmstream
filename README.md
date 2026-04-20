## shmstream
A two-process shared memory demo in C.
Producer writes a fake JPEG frame to POSIX shared memory.
Consumer reads it. Synchronized with a POSIX semaphore.
Mirrors how a camera driver and web server can share frame data.

## What i learned 
- I used the posix shared memory to communicate between two differnt process, where one acted as the producer like in the camera firmware one writes the jpeg frame, while the other consumer  who consumes this frames and send those to the required places.
- Used the semaphores to achieve the sync between the consumer and producer, so both of them dont act at the same time, so this avoid the race condition where the consumer tries to read the data while the producer overwrite the data. Semaphores solves this problem by syncronize them to do one action at a time. 
- Two process can share this memory region without any kernel involvment after the setup due to we mmap function to map this region as per the defined structure.
- The major advantage of this in comparison of the writing the jpeg frame at the tmp location or some known location that it reduce the no disk i/o and its more efficient and way faster than opening and reading the files. 

## Build system 
- Run the `make` to build, run the `make clean` for clean up of the genrated binaries.

## Instruction to run 
- After building using make, run the `producer` first in one terminal and  then the `consumer` in the second terminal.




