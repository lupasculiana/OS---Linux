#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define FIFO_NAME_READ "REQ_PIPE_79559"
#define FIFO_NAME_WRITE "RESP_PIPE_79559"
#define CONNECT "CONNECT"
#define EXIT "EXIT"
#define PING "PING"
#define PONG "PONG"
#define CREATE "CREATE_SHM"
#define WRITE "WRITE_TO_SHM"
#define MAP "MAP_FILE"
#define READ "READ_FROM_FILE_OFFSET"
#define SUCCESS "SUCCESS"
#define ERROR "ERROR"


unsigned int variant = 79559;
unsigned int connect_nr = strlen(CONNECT);
unsigned int nr_create = strlen(CREATE);
unsigned int nr_write = strlen(WRITE);
unsigned int nr_map = strlen(MAP);
unsigned int nr_read = strlen(READ);
unsigned int nr_error = strlen(ERROR);
unsigned int nr_success = strlen(SUCCESS);
char *sharedChar;
char *data = NULL;
int main()
{
    int fd_read = -1;
    int fd_write = -1;
    unsigned int nr;
    unsigned int nr_doi;
    unsigned int size_mem;
    unsigned int offset_add;
    unsigned int value_nr;
    unsigned int offset;
    unsigned int bytes_nr;
    int fd;
    off_t size;


    int shmFd;
    if(access(FIFO_NAME_WRITE, 0) == 0)
    {
        unlink(FIFO_NAME_WRITE);
    }
    // create the fifo for writing
    if(mkfifo(FIFO_NAME_WRITE, 0600) != 0)
    {
        perror("ERROR\ncannot create the response pipe\n");
        return 1;
    }

    //open fifo made from tester
    fd_read = open(FIFO_NAME_READ, O_RDONLY);
    if(fd_read == -1)
    {
        perror("ERROR\ncannot open the request pipe\n");
        return 1;
    }

    //open the writing fifo and make the connection
    fd_write = open(FIFO_NAME_WRITE, O_WRONLY);
    if((write(fd_write, &connect_nr, 1) != -1) && (write(fd_write, &CONNECT, strlen(CONNECT)) != -1))
    {
        printf("SUCCESS\n");
    }

    while(1)
    {
        //build the request string from tester's pipe
        read(fd_read, &nr, 1);
        char* req = (char*)malloc(nr*sizeof(char));
        read(fd_read, req, nr);
        //2.3
        if(strcmp("PING",req) == 0)
        {
            write(fd_write, &nr, 1);
            write(fd_write, &PING, strlen(PING));
            write(fd_write, &nr, 1);
            write(fd_write, &PONG, strlen(PONG));
            write(fd_write, &variant, sizeof(variant));
        }
        //2.4
        if(strcmp("CREATE_SHM",req)==0)
        {
            read(fd_read, &size_mem, sizeof(size_mem));


            shmFd = shm_open("/IyhL2H", O_CREAT | O_RDWR, 0664);
            if(shmFd < 0)
            {
                write(fd_write, &nr_create, 1);
                write(fd_write, &CREATE, strlen(CREATE));
                write(fd_write, &nr_error, 1);
                write(fd_write, &ERROR, strlen(ERROR));
                return 0;
            }
            ftruncate(shmFd, size_mem);
            sharedChar = (char*)mmap(0, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
            if(sharedChar == (void*)-1)
            {
                write(fd_write, &nr_create, 1);
                write(fd_write, &CREATE, strlen(CREATE));
                write(fd_write, &nr_error, 1);
                write(fd_write, &ERROR, strlen(ERROR));
                return 0;
            }

            write(fd_write, &nr_create, 1);
            write(fd_write, &CREATE, strlen(CREATE));
            write(fd_write, &nr_success, 1);
            write(fd_write, &SUCCESS, strlen(SUCCESS));


            //munmap((void*)sharedChar, sizeof(char));
            //sharedChar = NULL;
            //shm_unlink("/IyhL2H");

            //sharedChar = NULL;
            //close(shmFd);
        }
        //2.5
        if(strcmp("WRITE_TO_SHM",req)==0)
        {

            read(fd_read, &offset_add, sizeof(offset_add));
            read(fd_read, &value_nr, sizeof(value_nr));
            if(offset_add < 0 || offset_add +3 > 3383311)
            {
                write(fd_write, &nr_write, 1);
                write(fd_write, &WRITE, strlen(WRITE));
                write(fd_write, &nr_error, 1);
                write(fd_write, &ERROR, strlen(ERROR));
                return 0;
            }
            sharedChar[offset_add+3] = (char)(value_nr & 0xFF);
            sharedChar[offset_add+2] = (char)((value_nr >> 8) & 0xFF);
            sharedChar[offset_add+1] = (char)((value_nr >> 16) & 0xFF);
            sharedChar[offset_add] = (char)((value_nr >> 24) & 0xFF);

            write(fd_write, &nr_write, 1);
            write(fd_write, &WRITE, strlen(WRITE));
            write(fd_write, &nr_success, 1);
            write(fd_write, &SUCCESS, strlen(SUCCESS));
            //close(shmFd);
        }
        //2.6
        if(strcmp("MAP_FILE",req)==0)
        {
            read(fd_read, &nr_doi, 1);
            char* file_name = (char*)malloc(nr*sizeof(char));
            read(fd_read, file_name, nr_doi);

            fd = open(file_name, O_RDONLY);
            if(fd == -1)
            {
                write(fd_write, &nr_map, 1);
                write(fd_write, &MAP, strlen(MAP));
                write(fd_write, &nr_error, 1);
                write(fd_write, &ERROR, strlen(ERROR));
                return 0;
            }
            size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);

            data = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
            if(data == (void*)-1)
            {
                write(fd_write, &nr_map, 1);
                write(fd_write, &MAP, strlen(MAP));
                write(fd_write, &nr_error, 1);
                write(fd_write, &ERROR, strlen(ERROR));
                close(fd);
                return 0;
            }
            write(fd_write, &nr_map, 1);
            write(fd_write, &MAP, strlen(MAP));
            write(fd_write, &nr_success, 1);
            write(fd_write, &SUCCESS, strlen(SUCCESS));


        }
        //2.7
        if(strcmp("READ_FROM_FILE_OFFSET",req)==0)
        {
            read(fd_read, &offset, sizeof(offset));
            read(fd_read, &bytes_nr, sizeof(bytes_nr));
            if(offset+bytes_nr>size)
            {
                write(fd_write, &nr_read, 1);
                write(fd_write, &READ, strlen(READ));
                write(fd_write, &nr_error, 1);
                write(fd_write, &ERROR, strlen(ERROR));
                return 0;
            }
            for(int i=0; i<=bytes_nr-1; i++)
            {
                sharedChar[i]=data[offset+i];
            }

            write(fd_write, &nr_read, 1);
            write(fd_write, &READ, strlen(READ));
            write(fd_write, &nr_success, 1);
            write(fd_write, &SUCCESS, strlen(SUCCESS));

        }
        if(strcmp("EXIT",req) == 0)
        {
            close(fd_read);
            close(fd_write);
            free(req);
            req = NULL;
            close(shmFd);
            munmap(data, size);
            close(fd);

            //delete fifo
            unlink(FIFO_NAME_WRITE);
            return 0;
        }
        free(req);
        req= NULL;
        //close(shmFd);

        return 0;
    }
}
