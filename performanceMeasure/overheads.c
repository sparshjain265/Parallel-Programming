#include<stdio.h>
#include<pthread.h>
#include<sys/time.h>
#include<semaphore.h>

#define N 100

// some toy function
void* foo(void *a)
{
	int i, j;
	for(i = 0; i < 100; i++)
		j = i;
	for(j = 0; j < 100; j++)
		i = j;
	return NULL;
}

int main()
{
	struct timeval start, end;
	
	// calculate average time to run function foo
	struct timeval fooTime = {0, 0};
	for(int i = 0; i < N; i++)
	{
		struct timeval temp;
		gettimeofday(&start, NULL);
		foo(NULL);
		gettimeofday(&end, NULL);
		timersub(&end, &start, &temp);
		fooTime.tv_sec += temp.tv_sec;
		fooTime.tv_usec += temp.tv_usec;
	}
	fooTime.tv_sec /= N;
	fooTime.tv_usec /= N;
	// printf("%ld.%06ld\n", fooTime.tv_sec, fooTime.tv_usec);

	// calculate time from pthread create to pthread join, take average, then subtract the average time of function to calculate overhead
	FILE *create_join;
	create_join = fopen("create_join.txt", "a"); 
	for(int i = 1; i <=N; i++)
	{
		struct timeval create_join_overhead = {0, 0};
		pthread_t th[i];
		// start counting
		gettimeofday(&start, NULL);
		// create i threads
		for(int j = 0; j < i; j++)
			pthread_create(&th[j], NULL, foo, NULL);
		// join i threads
		for(int j = 0; j < i; j++)
			pthread_join(th[j], NULL);
		// stop counting
		gettimeofday(&end, NULL);
		timersub(&end, &start, &create_join_overhead);
		timersub(&create_join_overhead, &fooTime, &create_join_overhead);
		fprintf(create_join, "%ld.%06ld ", create_join_overhead.tv_sec, create_join_overhead.tv_usec);
	}
	fprintf(create_join, "\n");
	fclose(create_join);

	// overhead for mutex lock and unlock
	FILE *mutex;
	mutex = fopen("mutex.txt", "a");
	pthread_mutex_t lock_m;
	pthread_mutex_init(&lock_m, NULL);
	gettimeofday(&start, NULL);
	pthread_mutex_lock(&lock_m);
	foo(NULL);
	pthread_mutex_unlock(&lock_m);
	gettimeofday(&end, NULL);
	struct timeval mutex_overhead;
	timersub(&end, &start, &mutex_overhead);
	timersub(&mutex_overhead, &fooTime, &mutex_overhead);
	fprintf(mutex, "%ld.%06ld\n", mutex_overhead.tv_sec, mutex_overhead.tv_usec);
	fclose(mutex);

	// overhead for semaphone lock and unlock
	FILE *sema;
	sema = fopen("sema.txt", "a");
	sem_t lock_s;
	sem_init(&lock_s, 0, 0);
	gettimeofday(&start, NULL);
	sem_post(&lock_s);
	foo(NULL);
	sem_wait(&lock_s);
	gettimeofday(&end, NULL);
	struct timeval sema_overhead;
	timersub(&end, &start, &sema_overhead);
	timersub(&sema_overhead, &fooTime, &sema_overhead);
	fprintf(sema, "%ld.%06ld\n", sema_overhead.tv_sec, sema_overhead.tv_usec);
	fclose(sema);

	return 0;
}