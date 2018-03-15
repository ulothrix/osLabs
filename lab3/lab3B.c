#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h> 		//for usleep()

int durationFlag = 0; //-1: before, 0: default, 1: after

struct multiplePrintData {
	pthread_t id;
	int threadid;
	char printChar;
	int numOfTimes;
	double waitDuration;
};

int printCharMultipleTimes(char prCh, int count, double seconds)
{
	int i, countBefore = 0, countAfter = 0;

	for (i = 0; i < count; i++)
	{
		while (!durationFlag);

		if (-1 == durationFlag)
		{
			countBefore++;
		}
		else if (1 == durationFlag)
		{
			countAfter++;
		}

		fflush(stdout);
		printf("%c", prCh);
		usleep((int)(seconds * 1000000));
	}
	return countAfter;
}

void *printFunc(void *p)
{
	struct multiplePrintData *ip = (struct multiplePrintData*)p;

	printf("prFunc for thread%d   -> char: %c, times: %4d, sleep (seconds): %lf\n", (*ip).threadid, (*ip).printChar, (*ip).numOfTimes, (*ip).waitDuration);
	int delayed = printCharMultipleTimes((*ip).printChar, (*ip).numOfTimes, (*ip).waitDuration);

	printf("\nprFunc for thread%d   -> Done! (%3d print(s) were delayed.)\n", (*ip).threadid, delayed);
	pthread_exit(0);
}

void *durationCheckerFunc(void *p)
{
	double *ip = (double*)p;

	printf("durationCheckerFunc  -> Total duration (sec.s) of the program: %lf\n", *ip);
	printf("durationCheckerFunc  -> Starting the whole process.\n");
	durationFlag = -1;
	usleep((int)(*ip * 1000000));
	printf("\ndurationCheckerFunc  -> Time is up!\n");
	durationFlag = 1;

	pthread_exit(0);
}

main()
{
	pthread_t checkerThreadId;
	int numberOfThreads;
	double totalDuration;

	printf("Main Function        -> Enter the duration of the program (sec.s): ");
	scanf("%lf", &totalDuration);

	printf("Main Function        -> Enter the number of different characters : ");
	scanf("%d", &numberOfThreads);

	struct multiplePrintData *info;
	info = malloc(numberOfThreads * sizeof(struct multiplePrintData));

	printf("\nMain Function        -> Assigning values.\n");

	int i;
	for (i = 0; i < numberOfThreads; i++)
	{
		info[i].threadid = i + 1;

		printf("  %d-Enter char       : ", i + 1);
		scanf(" %c", &info[i].printChar);

		printf("  %d-Enter # of times : ", i + 1);
		scanf("%d", &info[i].numOfTimes);

		printf("  %d-Enter frequency  : ", i + 1);
		scanf("%lf", &info[i].waitDuration);

		printf("\n");
	}

	printf("Main Function        -> Creating the %d printer threads with argument passing.\n", numberOfThreads);
	for (i = 0; i < numberOfThreads; i++)
	{
		pthread_create(&info[i].id, NULL, printFunc, &info[i]);
	}

	sleep(1);
	printf("Main Function        -> Slept 1 sec. just before creating the checker thread with argument passing.\n");
	pthread_create(&checkerThreadId, NULL, durationCheckerFunc, &totalDuration);

	for (i = 0; i < numberOfThreads; i++)
	{
		pthread_join(info[i].id, NULL);
	}
	pthread_join(checkerThreadId, NULL);

	printf("Main Function        -> End of the program.\n");
}