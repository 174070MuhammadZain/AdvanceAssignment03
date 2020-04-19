#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h> 

sem_t mutex;
sem_t mutex1;
int PCP = 0;

struct LCP
{
    int id;
    int st;
};



// The function to be executed by all threads 
void* myThreadFun(void* vargp)
{

    sem_wait(&mutex);                         // enter in critical section  

    PCP++;                                 //increment corono paotents

    sem_post(&mutex);                      //crirtical section task complete
    sem_post(&mutex1);                     // Signal that 1 thread completed their work

    pthread_exit(NULL);
}



bool check(int nu, int j, int ar[])
{
    if (j == 0)
    {
        return true;
    }

    for (int k = 0; k < j; k++)
    {
        if (ar[k] == nu)
        {
            return false;
            break;
        }
    }

    return true;

}

void Randoms(struct LCP cp[])                         //this function randomnly makes corona patients
{
    int i, lower, upper, count;
    i = 0;
    lower = 0;
    upper = PCP;
    count = PCP / 2;
    int arr[count];
    while (i < count)
    {
        int num = ((rand() % upper));
        if (check(num, i, arr))
        {
            arr[i] = num;
            i++;
            cp[num].st = 1;

        }

        //printf("Status: %d\n",num);

    }



}




int test1()                                                 //just simple test for corona
{
    int num = (rand() %
        (1 - 0 + 1)) + 0;
}

int test2()
{
    int num = (rand() %
        (1 - 0 + 1)) + 0;
}








int main()
{
    int i;
    sem_t CP;
    sem_t FP;

    pthread_t tid;

    sem_init(&mutex, 0, 1);
    sem_init(&mutex1, 0, 0);

    sem_init(&CP, 1, 0);
    sem_init(&FP, 1, 0);


    //create n threads 
    for (i = 0; i < 10; i++)
    {
        pthread_create(&tid, NULL, myThreadFun, NULL);

        sem_wait(&mutex1);                            //wait for thread to complete
    }

    printf("Global: %d\n", PCP);        //print global function


    struct LCP cp[PCP];

    for (int i = 0; i < PCP; i++)
    {
        cp[i].id = i;
        cp[i].st = 0;
    }


    Randoms(cp);                //randomnly generate corona patients

    printf("Patient Before test\n");

    for (int i = 0; i < PCP; i++)
    {
        printf("Status: %d\n", cp[i].st);
    }



    for (int i = 0; i < PCP; i++)
    {
        if (cp[i].st == 1)
        {
            sem_post(&CP);                             //post the corona patient semaphore
        }
        else if (cp[i].st == 0)
        {
            sem_post(&FP);                            //post the non corona payient semaphore
        }

        else
        {
            PCP--;
        }


    }


    for (int i = 0; i < PCP; i++)
    {

        if (cp[i].st == 1)
        {
            if (!(test1() && test2()))                //if both test pass 
            {
                sem_wait(&CP);                        //wait corona patients
                cp[i].st = 0;
            }
        }

    }

    printf("Patient After test\n");

    for (int i = 0; i < PCP; i++)
    {
        printf("Status: %d\n", cp[i].st);
    }




    sem_destroy(&mutex);
    sem_destroy(&mutex1);
    sem_destroy(&CP);
    sem_destroy(&FP);
    return 0;
}

