//
// Created by ulothrix on 13.03.2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <zconf.h>

int numOfChars;
double duration,elapsed;
//time_t start, end;
int terminate = 1;
struct timespec start, finish;

struct multiplePrintData
{
    pthread_t id;
    int threadid;
    char printChar;
    int numOfTimes;
    double waitDuration;
};

void printCharMultipleTimes(char prCh, int count, double seconds){
    for (int i = 0; i < count; ++i) {
        fflush(stdout);
        printf("%c",prCh);
        usleep((int)(seconds*1000000));
    }
}
void *threadGivenTime(void* timeNeedtobeTaken){
//    start = time(NULL);
    clock_gettime(CLOCK_MONOTONIC, &start);
    double* tnt = (double*) timeNeedtobeTaken;
    printf("durationCheckerFunc -> Total duration (sec.s) of the program: %lf \n",(*tnt));
    while (terminate){
//        end = time(NULL);
//        elapsed = difftime(start,end);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        if (elapsed >= (*tnt)){
            terminate=0;
        }else{
            usleep(5000);
        }
    }
    printf("\ndurationCheckerFunc -> Time is up!\n");
    return 0;
}
void *threadCharacter(void *parameter){
    struct multiplePrintData *args = (struct multiplePrintData *)parameter;
    char printChar = args->printChar;
    int numOfTimes = args->numOfTimes;
    double waitDuration = args->waitDuration;
    printCharMultipleTimes(printChar,numOfTimes,waitDuration);
    return 0;
}
int main(){
    printf("Main function -> Enter the duration of the program (sec.s): ");
    scanf("%lf",&duration);
    printf("Main Function -> Enter the number of different characters : ");
    scanf("%d",&numOfChars);

    pthread_t threadTime, threadCh[numOfChars];

    printf("Main Function -> Assigning values.\n");

    struct multiplePrintData args[numOfChars];
    for (int i = 0; i < numOfChars; ++i) {
        printf("%d-Enter char : ",(i+1));
        scanf("%s",&args[i].printChar);
        printf("%d-Enter # of times : ",(i+1));
        scanf("%d",&args[i].numOfTimes);
        printf("%d-Enter frequency : ",(i+1));
        scanf("%lf",&args[i].waitDuration);
        args[i].threadid = i;
        args[i].id = (pthread_t) threadCh[i];
    }
    printf("Main Function -> Creating all the threads (1 checker & %d printer) with argument passing\n",numOfChars);

    pthread_create(&threadTime,NULL,threadGivenTime,(void *) &duration);
    for (int j = 0; j < numOfChars; ++j) {
        pthread_create(&threadCh[j],NULL,threadCharacter,(void *) &args[j]);
    }
    pthread_join(threadTime,NULL);
    printf("Main Function -> Aborting printing processes (if there is any)\n");

    for (int k = 0; k < numOfChars; ++k) {
        if (pthread_cancel(threadCh[k]) != 0){
            printf("Main Function -> Thread0 has already finished its job successfully.\n");
        }else{
            printf("Main Function -> Thread1 is aborted!\n");
        }
    }
    printf("Main Function -> End of the program\n");

}