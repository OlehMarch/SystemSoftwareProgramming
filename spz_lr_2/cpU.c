#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <linux/kernel.h>


void UFileCopy(char* newFileName, int empty)
{
	int inputFileDesc, outputFileDesc;
	char Buffer[256];
	ssize_t bytesOfInFile, bytesOfOutFile;

	inputFileDesc = open((empty == 1) ? "stock.file" : "stock2.file", O_RDONLY);
	if (inputFileDesc == -1)
	{
		return;
	}
	outputFileDesc = open(newFileName, O_WRONLY | O_CREAT, 0666);
	if (outputFileDesc == -1)
	{
		return;
	}

	while ((bytesOfInFile = read(inputFileDesc, &Buffer, 256)) > 0)
	{
		bytesOfOutFile = write(outputFileDesc, &Buffer, (size_t)bytesOfInFile);
		if (bytesOfOutFile != bytesOfInFile)
		{
			printf ("Write error!\n");
			return;
		}
	}
	
	close(inputFileDesc);
	close(outputFileDesc);
}

int main (int argc, char *argv [])
{	
	struct timespec start, end;
	double resultTime;
	
	clock_gettime(CLOCK_REALTIME, &start);
	UFileCopy("UFileCopyEmpty.file", 1);
	clock_gettime(CLOCK_REALTIME, &end);
	resultTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000;
	printf("-- Small File --\nU Time: %lf\n", resultTime);
		
	clock_gettime(CLOCK_REALTIME, &start);
	UFileCopy("UFileCopy.file", 0);
	clock_gettime(CLOCK_REALTIME, &end);
	resultTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000;
	printf("-- Large File --\nU Time: %lf\n", resultTime);
	
	printf("Press any key to continue...");
	getchar();
	return 0;
}


/*	ulong start = 0, end = 0;
	timer_t *timerID;
	struct sigevent sev;
	struct itimerspec timer;
	
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerID;
	
	if (timer_create(CLOCK_REALTIME, &sev, &timerID) == -1)
	{
		printf("Timer creation error!\n");
		return 1;
	}
	
	timer.it_value.tv_sec = 1 / 1000;
	timer.it_value.tv_nsec = 0;
	timer.it_interval.tv_sec = 1 / 1000;
	timer.it_interval.tv_nsec = 0;
	timer_settime(timerID, 0, &timer, NULL);
	
	start = time(NULL);
		printf("TimeS: %lu\n", start);
	UFileCopy("UFileCopyEmpty.file", 1);
	end = time(NULL) - start;
		printf("TimeE: %lu\n", (ulong)time(NULL));
	printf("-- Small File --\nU Time: %lu\n", end);
	
	start = (ulong)time(NULL);
		printf("\nTimeS: %lu\n", start);
	UFileCopy("UFileCopy.file", 0);
	end = (ulong)time(NULL) - start;
		printf("TimeE: %lu\n", (ulong)time(NULL));
	printf("-- Large File --\nU Time: %lu\n", end);
*/
