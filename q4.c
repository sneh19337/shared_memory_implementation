#include<stdio.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<semaphore.h>
#define q 1
    sem_t mutex;//creating a mutex for creating s link betweent the shared memory
    int  main( )
    {
        int *a, *b;
        char* s[]={"Hello."," This is 19 th of november."," The people are finding it really cool these"," days. The winter has finally arrived."," The festival of diwali is gone and rgis year no one can ","say that the diwali was as fun as in previous years. ","But the times will get good over time "};
        int i;
        key_t  key = 15;
        int    shmid_1, shmid_2;
        int status;
        //making two shared memory units one for writing the numbers and others for writing the words
        if ((shmid_1=shmget(key, 1000, 0640|IPC_CREAT)) == -1)//first shared memory allocation
        {
            perror("shmget shmid_1");
            return 1;}

        if ((shmid_2=shmget(IPC_PRIVATE, 20, 0640)) == -1)//second shared memory allocation
        {
            perror("shmget shmid_2");
            return 2;
        }

        if (fork() == 0) //creating child process

        {

            b = (int *) shmat(shmid_1, 0, 0);

            i=0;
            if (i < 10) {
                do {
                    sleep(1);
                    printf("\t\t\t  %d\n", b[0]);
                    printf("\t\t\t  %d\n", b[1]);
                    a = ( int *)shmat(shmid_2, 0, 0);

                    i++;
                } while (i < 10);
            }
            sem_init(&mutex,0,1);
            i=0;
            if (i < sizeof(s)) {
                do {

                    printf("\t\t\t  %s\n", s[i]);

                    i++;
                } while (i < sizeof(s));
            }
            sem_init(&mutex,0,1);
            a = (int *) shmat(shmid_1, 0, 0);

            a[0] = 1; a[1] = 3;
            i=0;
            if (i < 10) {
                do {
                    sleep(1);


                    a[0] = a[0] + a[q];
                    a[q] = a[0] + a[q];
                    i=i+1;
                } while (i < 10);
            }

            wait(&status);

            shmctl(shmid_1, IPC_RMID, 0);
            a = (int *) shmat(shmid_2, 0, 0);
            b = (int *) shmat(shmid_1, 0, 0);

            i=0;
            while (i<sizeof(s)-1) {

                i++;
            }


            shmdt(s);
            shmdt(b);

        }
        else {

            a = (int *) shmat(shmid_1, 0, 0);
            a[0] = 1; a[1] = 3;
            i=0;
            if (i < 10) {
                do {
                    sleep(1);
                    a[0] = a[0] + a[q];
                    a[q] = a[0] + a[q];
                    i=i+1;
                } while (i < 10);
            }
            wait(&status);

            shmctl(shmid_1, IPC_RMID, 0);
            a = (int *) shmat(shmid_2, 0, 0);


            i=0;
            if (i < sizeof(s) - 1) {
                do {

                    i++;
                } while (i < sizeof(s) - 1);
            }
            wait(&status);
            b = (int *) shmat(shmid_1, 0, 0);
            for( i=0; i< 10; i++) {


                printf("\t\t\t  %d\n",b[0]);
                printf("\t\t\t  %d\n",b[1]);

            }
            a = (int *) shmat(shmid_2, 0, 0);
            for( i=0;i<7; i++) {

                printf("\t\t\t  %s\n",s[i]);

            }
            shmdt(a);
            shmdt(s);
            shmctl(shmid_2, IPC_RMID, 0);
        }
    }