#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>

#define BINS 100
int hist[BINS];

struct argument
{
	int rank;
	long long start;
	long long end;
};

pthread_mutex_t mutexLock;
int p;
int flag = 0;
int* input;

void* useMutex(void* a)
{
	struct argument* args = (struct argument*) a;
	int rank = args->rank;
	long long start = args->start;
	long long end = args->end;
	for(int i = start; i < end; i++)
	{
		pthread_mutex_lock(&mutexLock);
		hist[input[i] % 100] += 1;
		pthread_mutex_unlock(&mutexLock);
	}
	return NULL;
}

void* useBusyWait(void *a)
{
	struct argument* args = (struct argument*) a;
	int rank = args->rank;
	long long start = args->start;
	long long end = args->end;
	for(int i = start; i < end; i++)
	{
		while(flag != rank);
		hist[input[i] % 100] += 1;
		flag = (flag + 1) % p;
	}
	return NULL;
}

void* useLocal(void *a)
{
	struct argument* args = (struct argument*) a;
	long long start = args->start;
	long long end = args->end;
	int *hist;
	hist = (int *)calloc(BINS, sizeof(int));
	for(int i = start; i < end; i++)
		hist[input[i] % 100] += 1;
	
	return (void *)hist;
}

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]); //size of input
	p = atoi(argv[2]); //number of threads
	input = (int *)calloc(n, sizeof(int));
	pthread_t th[p];
	struct argument args[p];

	struct timeval start, end, temp, busyWait = {0, 0}, mutex = {0, 0}, local = {0, 0};
	// take average over 10 runs
	int N = 10;
	for(int i = 0; i < N; i++)
	{
		// generate random input
		for(int j = 0; j < n; j++)
			input[i] = rand() % 100;
		
		// for mutex
		for(int j = 0; j < BINS; j++)
			hist[j] = 0;
		gettimeofday(&start, NULL);
		for(int j = 0; j < p; j++)
		{
			args[j].rank = j;
			args[j].start = (n/p) * j;
			args[j].end = (n/p) * (j+1);
			pthread_create(&th[j], NULL, useMutex, &args[j]);
		}
		for(int j = 0; j < p; j++)
			pthread_join(th[j], NULL);
		gettimeofday(&end, NULL);
		timersub(&end, &start, &temp);
		mutex.tv_sec += temp.tv_sec;
		mutex.tv_usec += temp.tv_usec;
		// printf("Mutex\n");

		// for busy wait
		for(int j = 0; j < BINS; j++)
			hist[j] = 0;
		gettimeofday(&start, NULL);
		for(int j = 0; j < p; j++)
		{
			args[j].rank = j;
			args[j].start = (n/p) * j;
			args[j].end = (n/p) * (j+1);
			pthread_create(&th[j], NULL, useBusyWait, &args[j]);
		}
		for(int j = 0; j < p; j++)
			pthread_join(th[j], NULL);
		gettimeofday(&end, NULL);
		timersub(&end, &start, &temp);
		busyWait.tv_sec += temp.tv_sec;
		busyWait.tv_usec += temp.tv_usec;
		// printf("Busy Wait\n");

		// for local
		for(int j = 0; j < BINS; j++)
			hist[j] = 0;
		gettimeofday(&start, NULL);
		for(int j = 0; j < p; j++)
		{
			args[j].rank = j;
			args[j].start = (n/p) * j;
			args[j].end = (n/p) * (j+1);
			pthread_create(&th[j], NULL, useLocal, &args[j]);
		}
		for(int j = 0; j < p; j++)
		{
			void *locH;
			pthread_join(th[j], &locH);
			int *locHist = (int *)locH;
			for(int k = 0; k < BINS; k++)
				hist[k] += locHist[k];
			free(locHist);
		}
		gettimeofday(&end, NULL);
		timersub(&end, &start, &temp);
		local.tv_sec += temp.tv_sec;
		local.tv_usec += temp.tv_usec;
		// printf("Local\n");
	}
	busyWait.tv_sec /= N;
	busyWait.tv_usec /= N;
	mutex.tv_sec /= N;
	mutex.tv_usec /= N;
	local.tv_sec /= N;
	local.tv_usec /= N;
	printf("%ld.%06ld\n", busyWait.tv_sec, busyWait.tv_usec);
	printf("%ld.%06ld\n", mutex.tv_sec, mutex.tv_usec);
	printf("%ld.%06ld\n", local.tv_sec, local.tv_usec);

	free(input);
	return 0;
}