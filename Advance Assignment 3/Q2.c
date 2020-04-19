#include <stdio.h> 
#include <semaphore.h> 
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>



int main()
{


    int id_sem = shmget(990, 1024, 0666 | IPC_CREAT);
    int id_cs = shmget(991, 1024, 0666 | IPC_CREAT);
    int id_sem2 = shmget(992, 1024, 0666 | IPC_CREAT);
    int id_sem3 = shmget(993, 1024, 0666 | IPC_CREAT);
    int id_sem4 = shmget(994, 1024, 0666 | IPC_CREAT);
    int id_sem5 = shmget(995, 1024, 0666 | IPC_CREAT);
    int id_sem6 = shmget(996, 1024, 0666 | IPC_CREAT);
    int id_cs1 = shmget(997, 1024, 0666 | IPC_CREAT);

    sem_t* empty1;
    sem_t* empty2;
    sem_t* full1;
    sem_t* full2;
    sem_t* sema1;
    sem_t* sema2;

    empty1 = (sem_t*)shmat(id_sem, NULL, 0);
    empty2 = (sem_t*)shmat(id_sem2, NULL, 0);
    full1 = (sem_t*)shmat(id_sem3, NULL, 0);
    full2 = (sem_t*)shmat(id_sem4, NULL, 0);
    sema1 = (sem_t*)shmat(id_sem5, NULL, 0);
    sema2 = (sem_t*)shmat(id_sem6, NULL, 0);



    char* buffer;
    char* bufferr;
    FILE* fptr;
    FILE* fptrr;
    char c, a;


    sem_init(empty1, 1, 1);
    sem_init(empty2, 1, 1);
    sem_init(full1, 1, 0);
    sem_init(full2, 1, 0);
    sem_init(sema1, 1, 1);
    sem_init(sema2, 1, 1);

    buffer = (char*)shmat(id_cs, NULL, 0);                         //buffer 1
    bufferr = (char*)shmat(id_cs1, NULL, 0);                       //buffer 2

    fptr = fopen("file1.txt", "r");                              //file opening
    fptrr = fopen("file2.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file 1\n");
        exit(0);
    }
    if (fptrr == NULL)
    {
        printf("Cannot open file 2\n");
        exit(0);
    }
    bool first = true;

    // Creating first fork 
    int n1 = fork();

    if (n1 > 0)
    {
     
        sem_wait(empty1);      //wait until buffer gets empty
       
        sem_wait(sema1);     //this is for producer critical section

        for (int i = 0; i < 10; i++)             //reading 10 characters from file
        {
            c = fgetc(fptr);
            if (c == EOF)
            {
                break;

            }
            else
            {
                buffer[i] = c;
                //  printf("%c",c);
                 // s=1;
            }
        }

        sem_post(sema1);
        sem_post(full1);                     //that it is full

    }

    else if (n1 == 0)
    {

        sem_wait(empty1);      //wait until buffer gets empty
        //cout<<"Hello";
        sem_wait(sema2);     //this is for producer critical section
        for (int i = 0; i < 10; i++)                      //reading ten charaters from file 2
        {
            a = fgetc(fptrr);
            if (a == EOF)
            {
                first = false;
                break;
            }
            else
            {
                buffer[i] = a;
                // printf("%c",a);
            }
        }

        sem_post(sema2);                 
        sem_post(full1);                       //signal buffer 1 gets full


    }

    int n2 = fork();

    if (n2 > 0)
    {
        sem_wait(full1);                         //waits for bufeer full
        sem_wait(empty2);
        strcpy(bufferr, buffer);                 //copy the buffer in buffer 2
        sem_post(empty1);                        //signal buffer 1 empty
        sem_post(full2);                         //signal that buffer 2 full

    }
    else if (n2 == 0)
    {

        sem_wait(full2);                         //waits for buffer 2 to full
        printf("%s", bufferr);
        sem_post(empty2);                        //signal buffer 2 empty

    }




    sem_destroy(empty1);
    sem_destroy(empty2);
    sem_destroy(full1);
    sem_destroy(full2);
    sem_destroy(sema1);
    sem_destroy(sema2);

    shmdt(empty1);
    shmdt(empty2);
    shmdt(full1);
    shmdt(full2);
    shmdt(sema1);
    shmdt(sema2);
    shmdt(buffer);
    shmdt(bufferr);
    shmctl(id_sem, IPC_RMID, NULL);
    shmctl(id_sem2, IPC_RMID, NULL);
    shmctl(id_sem3, IPC_RMID, NULL);
    shmctl(id_sem4, IPC_RMID, NULL);
    shmctl(id_sem5, IPC_RMID, NULL);
    shmctl(id_sem6, IPC_RMID, NULL);
    shmctl(id_cs, IPC_RMID, NULL);
    shmctl(id_cs1, IPC_RMID, NULL);

    return 0;
}

