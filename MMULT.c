#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <sys/shm.h>
#include <sys/wait.h>

#include "shm_com.h"
#include "array_functions.h"                // Header file containing array functions

#define MICRO_SEC_IN_SEC 1000000

static int complete_processes = 0;          // Number of child processes that have completed
static int complete = 0;                    // All processes complete flag

void trigger_func(int sig){                 // function called by sigaction
    if (complete_processes == 3)            // If all four processes have completed ->
        complete = 1;                       // Set complete flag to 1
    complete_processes++;                  
}

int main()
{
    // Define arrays -> Mandatory test Data used, secondary test commented below
    int m[4][4] =  {
        {20, 20, 50, 40},
        {10, 6, 70, 8},
        {40, 3, 2, 10},
        {8, 7, 6, 5}
    };

    int n[4][4] ={
        {10, 30, 50, 70},
        {1, 3, 6, 8},
        {9, 5, 5, 7},
        {8, 6, 4, 2}
    };

    // Second test Data

//    int m[4][4] = {
//        {4, 7, 2, 4},
//        {50, 30, 40, 90},
//        {30, 70, 80, 90},
//        {80, 60, 50, 20}
//    };

//    int n[4][4] = {
//        70, 10, 90, 50},
//        {20, 30, 80, 80},
//        {70, 40, 30, 90},
//        {80, 50, 90, 40}
//    };


    // Structs for timing execution
    struct timeval start, end;

    printf("Matrix Multiplication program starting...\n");

    /* Set up Shared Memory */
    void *shared_memory = (void *) 0;
    struct shared_use_st *shared_stuff;         // Place structure "mask" on shared memory

    int shmid;
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    /* Handle errors thrown by shmget */
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", shared_memory);

    /* Place struct mask on shared memory */
    shared_stuff = (struct shared_use_st *) shared_memory;

    // Copy arrays defined in main into shared memory
    copy_array(m, shared_stuff->array_m);
    copy_array(n, shared_stuff->array_n);

    // Print arrays M and N in shared memory
    print_array('M', shared_stuff->array_m);
    print_array('N', shared_stuff->array_n);

    /* Set up Sigaction */
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_handler = trigger_func;      // Set event handler
    sigemptyset(&act.sa_mask);

    // Forking processes
    gettimeofday(&start, NULL); 		// Get time of day before fork()

    pid_t pid;
    
    int process_number = -1;     // Number of process created, corresponds to column for child

    for(int p = 0; p < 4; p++){
        process_number++;       // Increment process number to be contained in next clone created by fork
                                // This number is a constant for child, doesn't need to be in shared memory
        pid = fork();
        switch(pid) {
            case -1:
                /* Failure */
                perror("fork failed");
                exit(EXIT_FAILURE);
            case 0:
                /* Child Process */
                printf("Child process: P%d, working with row %d\n", process_number, process_number);
                for(int i = 0; i < 4; i++){
                    for(int j = 0; j < 4; j++){
                        shared_stuff->array_q[process_number][i] += (shared_stuff -> array_m[process_number][j])
                                                                    * (shared_stuff -> array_n[j][i]);
                    }                                                                 
                }
                shared_stuff->ranked[process_number] = highest_row_value(shared_stuff->array_q[process_number]);
                kill(getppid(), SIGALRM);
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    for(int p = 0; p < 4; p++){
         sigaction(SIGALRM, &act, 0);     // uses sigaction instead of signal handler 
        pause();                          // Wait until child process is done
    }     

    if(complete){
        printf("\n");
        print_array('Q', shared_stuff -> array_q);
        printf("Largest value in Q: %d\n", highest_row_value(shared_stuff->ranked));
        gettimeofday(&end, NULL);
    }

    printf("Program completion time: %ld micro sec\n", ((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) -
							(start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));
    printf("Program terminating\n");
        
    /* Handle shared memory error*/
    if(shmdt (shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if(shmctl(shmid, IPC_RMID, 0) == -1){
        fprintf(stderr, "shmctl(IPC_RMID) failed \n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}